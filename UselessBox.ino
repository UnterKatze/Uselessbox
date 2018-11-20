#include "I2Cdev.h"
#include "MPU6050.h"
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 accelgyro;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo1;
Servo servo2;

int delayTime = 250;
int16_t xAxis, yAxis, zAxis;
int xResult, yResult, zResult;
double xOffset = 0, yOffset = 0, zOffset = 0;
double xRange = 16300.0, yRange = 16300.0, zRange = 32600.0;
double xMaxAngle = 90.0, yMaxAngle = 90.0, zMaxAngle = 180.0;
int GroundSwitch = 7;
int FunctionSwitch = 8;
int servo1StartPos = 0;
int servo2StartPos = 0;
int maxServo1Angle = 60;
int servo2TriggerAngle = 40;
int servo2SwitchHit = 45;
int servo1Pin = 3;
int servo2Pin = 5;


#define OUTPUT_READABLE_ACCELGYRO

void setup()
{
  pinMode(GroundSwitch, OUTPUT);
  pinMode(FunctionSwitch, INPUT);
  digitalWrite(GroundSwitch, LOW);
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(servo1StartPos);
  servo2.write(servo2StartPos);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Updating Offsets");
  accelgyro.initialize();
  delay(1000);

  for (int i = 1; i <= 100; i++)
  {
    accelgyro.getAcceleration(&xAxis, &yAxis, &zAxis);
    xOffset = xOffset + xAxis;
    yOffset = yOffset + yAxis;
    zOffset = zOffset + zAxis;
    delay(50);
  }

  xOffset = xOffset / 100.0;
  yOffset = yOffset / 100.0;
  zOffset = zOffset / 100.0;
  resetDisplay();
  digitalWrite(GroundSwitch, HIGH);
}


void loop()
{
  if (FunctionSwitch == HIGH)
  {

    for (int i = 0; i <= maxServo1Angle; i + 2)
    {
      accelgyro.getAcceleration(&xAxis, &yAxis, &zAxis);
      xResult = ((xAxis - xOffset) / xRange) * xMaxAngle;
      yResult = ((yAxis - yOffset) / yRange) * yMaxAngle;
      zResult = ((zAxis - zOffset) / zRange) * zMaxAngle;
      
      if (i % 10 == 0)
      {
        resetDisplay();
        lcd.setCursor(0, 0);
        lcd.print("x: "); 
        lcd.print(xResult);

        lcd.setCursor(8, 0);
        lcd.print("y: "); 
        lcd.print(yResult);

        lcd.setCursor(0, 1);
        lcd.print("z: ");
        lcd.print(zResult);
      }

      servo1.write(i);
      delay(50);

      if (xResult >= servo2TriggerAngle)
      {
        servo2.write(servo2SwitchHit);
        delay(333);
        servo2.write(servo2StartPos);
      }
      
    }

    for (int j = maxServo1Angle; j == 0; j - 2)
    {
      accelgyro.getAcceleration(&xAxis, &yAxis, &zAxis);
      xResult = ((xAxis - xOffset) / xRange) * xMaxAngle;
      yResult = ((yAxis - yOffset) / yRange) * yMaxAngle;
      zResult = ((zAxis - zOffset) / zRange) * zMaxAngle;

      if (j % 10 == 0)
      {
        resetDisplay();
        lcd.setCursor(0, 0);
        lcd.print("x: ");  
        lcd.print(xResult);

        lcd.setCursor(8, 0);
        lcd.print("y: ");   
        lcd.print(yResult);

        lcd.setCursor(0, 1);
        lcd.print("z: ");  
        lcd.print(zResult);
      }
      
      servo1.write(j);
      delay(50);     
    }
  }
}

void resetDisplay (void) {
  lcd.setCursor(0, 0);
  lcd.println("                ");
  lcd.setCursor(0, 1);
  lcd.println("                ");
}
