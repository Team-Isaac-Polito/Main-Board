#include <Wire.h>
#include "main_I2C.h"

#define I2C_SLAVE_ADDRESS 48
#define I2C_SDA_PIN 17
#define I2C_SCL_PIN 18

MainI2C mainI2C(&Wire, I2C_SLAVE_ADDRESS);

float leftSpeed = 65.0;  // Set to a higher value for noticeable movement
float rightSpeed = 65.0;

float leftTelemetry, rightTelemetry;

void setup() {
  Serial.begin(115200);

  // Initialize I2C communication
  mainI2C.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  delay(1000);  // Small delay for stability

  // Send initial motor speeds
  mainI2C.sendMotorSpeeds(leftSpeed, rightSpeed);

  // Request telemetry data
  mainI2C.requestTelemetry(&leftTelemetry, &rightTelemetry);
}

void loop() {
  // Periodically request telemetry data from the slave
  delay(2000);
  mainI2C.requestTelemetry(&leftTelemetry, &rightTelemetry);
}
