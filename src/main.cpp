// ServoOverdone - PlatformIO (VS Code) version
// Based on Koepel's ServoOverdone example (Public Domain)

#include <Arduino.h>
#include <Servo.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define NUM_SERVOS 32
Servo myServo[NUM_SERVOS];

void showPointer(int s);
void GenerateDiagram(); // optional (not used by default)

void setup()
{
  // Attach pins from the Arduino Mega board to the Servo objects.
  // Starting from pin 22, there happen to be exactly 32 pins.
  for (int i = 0; i < NUM_SERVOS; i++)
  {
    myServo[i].attach(i + 22); // pin 22 up to 53 is 32 pins
  }

  // যদি diagram.json generate করতে চাও, নিচের লাইনটা uncomment করো:
  // GenerateDiagram();
}

void loop()
{
  // Sequence one: random angles
  for (int a = 0; a < 15; a++)
  {
    for (int i = 0; i < NUM_SERVOS; i++)
    {
      myServo[i].write(random(0, 181));
      delay(2);
    }
    delay(150);
  }

  // Sequence two: all servos same angle sweep
  for (int i = 0; i < NUM_SERVOS; i++)
  {
    myServo[i].write(0);
  }
  delay(1000);

  for (int a = 0; a < 3; a++)
  {
    for (int r = 0; r <= 180; r++)
    {
      for (int i = 0; i < NUM_SERVOS; i++)
      {
        myServo[i].write(r);
      }
      delay(6);
    }
    for (int r = 180; r >= 0; r--)
    {
      for (int i = 0; i < NUM_SERVOS; i++)
      {
        myServo[i].write(r);
      }
      delay(6);
    }
  }

  // Sequence three: rotating wave
  for (int a = 0; a < 6; a++)
  {
    for (int i = 0; i < NUM_SERVOS; i++)
    {
      for (int j = 0; j < NUM_SERVOS; j++)
      {
        int d = j - i;
        if (d < 0)
          d = -d;
        if (d > (NUM_SERVOS / 2))
          d = NUM_SERVOS - d;

        int angle = 90 - (10 * d);
        if (angle < 0)
          angle = 0;

        myServo[j].write(angle);
      }
      delay(40);
    }
  }

  // Sequence four: "compass"
  int pointer = NUM_SERVOS * 3 / 4;
  showPointer(pointer);
  delay(1000);

  for (int i = 0; i < 5; i++)
  {
    showPointer(--pointer);
    delay(150);
  }
  delay(200);

  for (int i = 0; i < 9; i++)
  {
    showPointer(++pointer);
    delay(150);
  }
  delay(200);

  for (int i = 0; i < 5; i++)
  {
    showPointer(--pointer);
    delay(150);
  }
  delay(200);

  for (int i = 0; i < 4; i++)
  {
    showPointer(++pointer);
    delay(150);
  }
  delay(160);

  for (int i = 0; i < 2; i++)
  {
    showPointer(--pointer);
    delay(150);
  }
  delay(80);

  for (int i = 0; i < 1; i++)
  {
    showPointer(++pointer);
    delay(150);
  }

  delay(2000);
}

void showPointer(int s)
{
  int pointerA = s % NUM_SERVOS;
  int pointerB = (s + 1) % NUM_SERVOS;
  int tailA = (s + 16) % NUM_SERVOS;
  int tailB = (s + 17) % NUM_SERVOS;

  myServo[pointerA].write(180 - 56);
  myServo[pointerB].write(56);

  myServo[tailA].write(95);
  myServo[tailB].write(85);

  int n = (NUM_SERVOS / 2) - 2;

  // right of pointer
  int start = pointerB + 1;
  for (int i = 0; i < n; i++)
  {
    int j = (start + i) % NUM_SERVOS;
    myServo[j].write(2);
  }

  // left of pointer
  start = tailB + 1;
  for (int i = 0; i < n; i++)
  {
    int j = (start + i) % NUM_SERVOS;
    myServo[j].write(178);
  }
}

// Optional: Generate diagram.json via Serial output (advanced)
void GenerateDiagram()
{
  Serial.begin(115200);

  Serial.print("{\n");
  Serial.print("  \"version\": 1,\n");
  Serial.print("  \"author\": \"Generated\",\n");
  Serial.print("  \"editor\": \"wokwi\",\n");
  Serial.print("  \"parts\": [\n");

  Serial.print("    {\n");
  Serial.print("      \"type\": \"wokwi-arduino-mega\",\n");
  Serial.print("      \"id\": \"mega\",\n");
  Serial.print("      \"top\": 270,\n");
  Serial.print("      \"left\": 185,\n");
  Serial.print("      \"attrs\": {}\n");
  Serial.print("    },\n");

  for (int i = NUM_SERVOS - 1; i >= 0; i--)
  {
    float rotate = float(i) * (360.0 / float(NUM_SERVOS));
    float rad = rotate / 360.0 * 2.0 * M_PI;
    float top = (300.0 * sin(rad)) + 300.0;
    float left = (300.0 * cos(rad)) + 300.0;

    Serial.print("    {\n");
    Serial.print("      \"type\": \"wokwi-servo\",\n");
    Serial.print("      \"id\": \"servo");
    Serial.print(i);
    Serial.print("\",\n");
    Serial.print("      \"top\": ");
    Serial.print(top);
    Serial.print(",\n");
    Serial.print("      \"left\": ");
    Serial.print(left);
    Serial.print(",\n");
    Serial.print("      \"rotate\": ");
    Serial.print(rotate);
    Serial.print(",\n");
    Serial.print("      \"attrs\": { \"hornColor\": \"Red\" }\n");
    Serial.print("    }");
    if (i != 0)
      Serial.print(",");
    Serial.print("\n");
  }

  Serial.print("  ],\n");
  Serial.print("  \"connections\": [\n");

  for (int i = 0; i < NUM_SERVOS; i++)
  {
    int j = i + 1;
    if (j == NUM_SERVOS)
      j = 0;

    Serial.print("    [ \"servo");
    Serial.print(i);
    Serial.print(":V+\", \"servo");
    Serial.print(j);
    Serial.print(":V+\", \"Red\", [] ],\n");

    Serial.print("    [ \"servo");
    Serial.print(i);
    Serial.print(":GND\", \"servo");
    Serial.print(j);
    Serial.print(":GND\", \"Black\", [] ],\n");

    Serial.print("    [ \"mega:");
    Serial.print(i + 22);
    Serial.print("\", \"servo");
    Serial.print(i);
    Serial.print(":PWM\", \"Green\", [ ] ],\n");
  }

  Serial.print("    [ \"mega:GND.2\", \"servo9:GND\", \"Black\", [ ] ],\n");
  Serial.print("    [ \"mega:5V\", \"servo9:V+\", \"Red\", [ ] ]\n");

  Serial.print("  ]\n");
  Serial.print("}\n");
}