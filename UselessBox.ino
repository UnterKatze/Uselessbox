#include "I2Cdev.h"
#include "MPU6050.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 accelgyro;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int delayTime = 333;
int16_t xAxis, yAxis, zAxis;
int xResult, yResult, zResult;
double xOffset = 0, yOffset = 0, zOffset = 0;
double xRange = 16300.0, yRange = 16300.0, zRange = 32600.0;
double xMaxAngle = 90.0, yMaxAngle = 90.0, zMaxAngle = 180.0;

#define OUTPUT_READABLE_ACCELGYRO

void setup()
{
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

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
}

void loop()
{
  accelgyro.getAcceleration(&xAxis, &yAxis, &zAxis);

  lcd.setCursor(0, 0);
  lcd.print("x: ");
  xResult = ((xAxis - xOffset) / xRange) * xMaxAngle;
  lcd.print(xResult);

  lcd.setCursor(8, 0);
  lcd.print("y: ");
  yResult = ((yAxis - yOffset) / yRange) * yMaxAngle;
  lcd.print(yResult);

  lcd.setCursor(0, 1);
  lcd.print("z: ");
  zResult = ((zAxis - zOffset) / zRange) * zMaxAngle;
  lcd.print(zResult);

  delay(delayTime);
  resetDisplay();

}

void resetDisplay (void) {
  lcd.setCursor(0, 0);
  lcd.println("                ");
  lcd.setCursor(0, 1);
  lcd.println("                ");
}
