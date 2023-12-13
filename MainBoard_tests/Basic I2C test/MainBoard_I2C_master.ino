/*
  I2C Master Demo for MainBoard
  Master sends character and gets reply from Slave
  DroneBot Workshop 2019
  https://dronebotworkshop.com
*/

// Include Arduino Wire library for I2C
#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 48

// Define Slave answer size
#define ANSWERSIZE 5

void setup() {

  // Pins connected to MotorBoard (I2C0)
  // Initialize I2C communications as Master
  Wire.begin(17,18,100000);
  
  // Setup serial monitor
  Serial.begin(9600);
  Serial.println("I2C Master Demonstration");
}

void loop() {
  delay(50);
  Serial.println("Write data to slave");
  
  // Write a charatre to the Slave
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(0);
  byte error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.println(error);
  Serial.println("Receive data");
  
  // Read response from Slave
  // Read back 5 characters
  Wire.requestFrom(SLAVE_ADDR,ANSWERSIZE);
  
  // Add characters to string
  String response = "";
  while (Wire.available()) {
      char b = Wire.read();
      response += b;
  } 
  
  // Print to Serial Monitor
  Serial.print("response: ");
  Serial.println(response);
}
