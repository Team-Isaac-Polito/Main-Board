#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x08
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// Create a custom I2C instance
TwoWire I2C_Master = TwoWire(0);  // Use bus 0 for the I2C

float leftSpeed = 0.5;  // Example speed for left motor
float rightSpeed = 0.7; // Example speed for right motor

void setup() {
  Serial.begin(115200);

  // Initialize I2C as master on custom pins
  I2C_Master.begin(I2C_SDA_PIN, I2C_SCL_PIN, 100000);  // 100kHz clock

  delay(1000);  // Small delay for stability

  // Send motor speed data to the slave
  sendMotorSpeeds(leftSpeed, rightSpeed);

  // Request telemetry data from the slave
  requestTelemetry();
}

void loop() {
  // Add logic to periodically update speeds or request telemetry data
  delay(2000);  // Request telemetry every 2 seconds for testing
  requestTelemetry();
}

// Function to send motor speeds to the slave
void sendMotorSpeeds(float left, float right) {
  I2C_Master.beginTransmission(I2C_SLAVE_ADDRESS);
  
  // Send two float values for left and right motor speeds
  I2C_Master.write((byte*)&left, sizeof(float));
  I2C_Master.write((byte*)&right, sizeof(float));

  I2C_Master.endTransmission();

  Serial.println("Sent motor speeds to slave:");
  Serial.print("Left motor speed: ");
  Serial.println(left);
  Serial.print("Right motor speed: ");
  Serial.println(right);
}

// Function to request telemetry from the slave
void requestTelemetry() {
  I2C_Master.requestFrom(I2C_SLAVE_ADDRESS, 2 * sizeof(float));

  if (I2C_Master.available() == 2 * sizeof(float)) {
    float leftSpeedTelemetry, rightSpeedTelemetry;
    
    // Read the received telemetry data (two floats)
    I2C_Master.readBytes((char*)&leftSpeedTelemetry, sizeof(float));
    I2C_Master.readBytes((char*)&rightSpeedTelemetry, sizeof(float));

    // Print telemetry data
    Serial.println("Received telemetry data from slave:");
    Serial.print("Left motor speed: ");
    Serial.println(leftSpeedTelemetry);
    Serial.print("Right motor speed: ");
    Serial.println(rightSpeedTelemetry);
  }
}
