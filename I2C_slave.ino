#include <Wire.h>

byte slaveaddress = 0x30;
void setup() {
    Serial.begin(115200);
    delay(5000);
    //Wire.begin();
    //I2C0 channel needs Wire, I2C1 channel needs Wire1
    //Set pins before Wire.begin()
    if(!Wire.setSCL(9)){
      Serial.println("setscl error");
      while(1);
    }
    if(!Wire.setSDA(8)){
      Serial.println("setsda error");
      while(1);
    }
    Wire.begin(slaveaddress); //begins as a slave on address 0x30
    Wire.onReceive(recv);
    Wire.onRequest(req);
}

static char buff[100];
void loop() {
  Serial.print("im alive on address 0x");
  if (slaveaddress<16)
        Serial.print("0");
  Serial.println(slaveaddress, HEX);
  delay(1000);
}

// These are called in an **INTERRUPT CONTEXT** which means NO serial port
// access (i.e. Serial.print is illegal) and no memory allocations, etc.


// Called when the I2C slave gets written to
void recv(int len) {
    int i;
    // Just stuff the sent bytes into a global the main routine can pick up and use
    for (i=0; i<len; i++) buff[i] = Wire.read();
    buff[i] = 0;
}


// Called when the I2C slave is read from
void req() {
    static int ctr = 765;
    char buff[7];
    // Return a simple incrementing hex value
    sprintf(buff, "%06X", (ctr++) % 65535);
    Wire.write(buff, 6);
}
