#include "main_I2C.h"

// Constructor to initialize with TwoWire instance and slave address
MainI2C::MainI2C(TwoWire& wire, int slaveAddress)
  : wireInstance(wire), slaveAddress(slaveAddress) {}

// Function to initialize I2C as master
void MainI2C::begin(int sdaPin, int sclPin, uint32_t frequency) {
  wireInstance.begin(sdaPin, sclPin, frequency);
}

// Function to send motor speeds to the slave
void MainI2C::sendMotorSpeeds(float leftSpeed, float rightSpeed) {
  wireInstance.beginTransmission(slaveAddress);

  // Send two float values for left and right motor speeds
  wireInstance.write((byte*)&leftSpeed, sizeof(float));
  wireInstance.write((byte*)&rightSpeed, sizeof(float));

  wireInstance.endTransmission();

  Serial.println("Sent motor speeds to slave:");
  Serial.print("Left motor speed: ");
  Serial.println(leftSpeed);
  Serial.print("Right motor speed: ");
  Serial.println(rightSpeed);
}

// Function to request telemetry from the slave
void MainI2C::requestTelemetry(float* leftSpeedTelemetry, float* rightSpeedTelemetry) {
  wireInstance.requestFrom(slaveAddress, 2 * sizeof(float));

  if (wireInstance.available() == 2 * sizeof(float)) {
    // Read the received telemetry data (two floats)
    wireInstance.readBytes((char*)leftSpeedTelemetry, sizeof(float));
    wireInstance.readBytes((char*)rightSpeedTelemetry, sizeof(float));

    Serial.println("Received telemetry data from slave:");
    Serial.print("Left motor speed: ");
    Serial.println(*leftSpeedTelemetry);
    Serial.print("Right motor speed: ");
    Serial.println(*rightSpeedTelemetry);
  }
}
