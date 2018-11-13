# Uselessbox
A Uselessbox based on Arduino UNO and MPU6050 sensor

Maybe you know the common Uselessbox. If you press a switch at the top of the box a
part of the box is opened by a servo motor and a robot-arm (2nd servo motor) leaves the box and changes the position 
of the switch again.
Now the robot-arm reenters the box. You can press the button again.
This version uses the common MPU6050 accelerometer/gyroscope sensor to measure the angle 
of the opening part and starting the secound servo motor which changes the position of the switch.
The angle of the part which is opening will be displayed on an 16x2 I2C LCD.


Parts needed:
- Arduino Uno
- MPU6050 accelerometer/gyroscope sensor
- LiquidCrystal 16x2 I2C display
- 2 servo motors
- 1 switch
