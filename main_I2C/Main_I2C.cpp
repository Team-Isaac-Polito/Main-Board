#include "Main_I2C.h"

// Constructor to initialize with TwoWire instance and slave address
MainI2C::MainI2C(TwoWire* wire, int slaveAddress)
  : wireInstance(wire), slaveAddress(slaveAddress) {}

//Function to initialize I2C as master
void MainI2C::begin(int sdaPin, int sclPin, uint32_t frequency) {
  wireInstance->begin(sdaPin, sclPin, frequency);
}

// Function to send motor speeds to the slave
void MainI2C::sendMotorSpeeds(float leftSpeed, float rightSpeed) {
  wireInstance->beginTransmission(slaveAddress);

  // Send two float values for left and right motor speeds
  float buffer[2];
  buffer[0] = leftSpeed;
  buffer[1] = rightSpeed;

  wireInstance->write((uint8_t*)buffer, 2*sizeof(float));

  wireInstance->endTransmission();
}

// Function to request telemetry from the slave
void MainI2C::requestTelemetry(float* leftSpeedTelemetry, float* rightSpeedTelemetry) {
  wireInstance->requestFrom(slaveAddress, 2 * sizeof(float));

  if (wireInstance->available() == 2 * sizeof(float)) {
    // Read the received telemetry data (two floats)
    float buffer[2];
    wireInstance->readBytes((uint8_t*)buffer, 2*sizeof(float));
    *leftSpeedTelemetry = buffer[0];
    *rightSpeedTelemetry = buffer[1];
  }
}
