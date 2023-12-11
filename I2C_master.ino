#include <Wire.h>

void printhex(byte address) {
  if (address < 16)
    Serial.print("0");
  Serial.println(address, HEX);
}


void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;  // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");

  if (!Wire.begin(8, 9, 100000)) {
    Serial.println("Error with i2c begin");
    while (1)
      ;
  }
}


void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Serial.print("Scanning address 0x");
    printhex(address);

    Wire.beginTransmission(address);
    //Wire.write(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      printhex(address);
      Serial.println(" !");

      nDevices++;
    } else if (error == 1) {
      Serial.print("Error: data too long to fit in transmit buffer at address 0x");
      printhex(address);
    } else if (error == 2) {
      Serial.print("Error: received NACK on transmit of address at address 0x");
      printhex(address);
    } else if (error == 3) {
      Serial.print("Error: received NACK on transmit of data at address 0x");
      printhex(address);
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      printhex(address);
    } else if (error == 5) {
      Serial.print("Timeout at address 0x");
      printhex(address);
    } else  //should never enter here
    {
      Serial.print("Unhandled error at address 0x");
      printhex(address);
      Serial.print("Error is ");
      Serial.println(error);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("Found some devices\n");

  delay(2000);  // wait 5 seconds for next scan
}
