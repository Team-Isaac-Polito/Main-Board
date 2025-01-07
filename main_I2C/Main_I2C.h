#ifndef Main_I2C_h
#define Main_I2C_h

#include <Wire.h>

class MainI2C {
  public:
    MainI2C(TwoWire* wire, int slaveAddress);
    void begin(int sdaPin, int sclPin, uint32_t frequency = 100000);  // Initialize I2C with custom pins
    void sendMotorSpeeds(float leftSpeed, float rightSpeed);          // Send motor speeds to the slave
    void requestTelemetry(float* leftSpeedTelemetry, float* rightSpeedTelemetry);  // Request telemetry data

  private:
    TwoWire* wireInstance;
    int slaveAddress;
};

#endif
