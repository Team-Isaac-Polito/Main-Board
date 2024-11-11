#include <Wire.h>
#include "main_I2C.h"

#define I2C_SLAVE_ADDRESS 0x08
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// Create a custom I2C instance and MainI2C object
TwoWire I2C_Master = TwoWire(0);
MainI2C mainI2C(I2C_Master, I2C_SLAVE_ADDRESS);

float leftSpeed = 65.0;  // Set to a higher value for noticeable movement
float rightSpeed = 65.0;

void setup() {
  Serial.begin(115200);

  // Initialize I2C communication
  mainI2C.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  delay(1000);  // Small delay for stability

  // Send initial motor speeds
  mainI2C.sendMotorSpeeds(leftSpeed, rightSpeed);

  // Request telemetry data
  float leftTelemetry, rightTelemetry;
  mainI2C.requestTelemetry(&leftTelemetry, &rightTelemetry);
}

void loop() {
  // Periodically request telemetry data from the slave
  delay(2000);
  float leftTelemetry, rightTelemetry;
  mainI2C.requestTelemetry(&leftTelemetry, &rightTelemetry);
}
