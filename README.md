# MainBoard
This is the code for the second PCB developed by team Isaac to handle modules of Rese.Q MK2. It is built around ESP32 and can control multiple peripherals with different communication protocols.

## Source code
Needs to be compiled with the device `ESP32-S3-Box` on Arduino IDE or similar.
Currently being developed to integrate the fundamental code for the traction system **TestCAN_I2C** with a meaningful control of the state leds. More sensors and functionalities will follow.

## Test codes
- **I2C_1byte_Test** is a very basic code to handle the transmission of a single byte between MainBoard and MotorsBoard. Through a single byte only unsigned values can be transmitted,
   so with this code the motors would move only in one direction. Use this only as a basic test.
- **TestCan** use this code to test the CAN transciever on the board
- **TestServo** is currently WIP. Taken from PicoLowLevel, must be ajusted for compatibility with the ESP32
- **TestStateLEDs** controls the two RGB LED on the board. Can be used also to control a full led strip connected to the `RGB LEDs` output on the board, by specifying the number of LEDs.
- **TestCAN_I2C** is the latest code used for the robot movement. It receives and parses motor values from the Jetson and sends them to the MotorsBoard via I2C. Values are received as 64bits floats and sent on I2C as short int.
