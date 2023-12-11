// Simple I2C master and slave demo - Earle F. Philhower, III
// Released into the public domain
// 
// Using both onboard I2C interfaces, have one master and one slave
// and send data both ways between them
//
// To run, connect GPIO0 to GPIO2, GPIO1 to GPIO3 on a single Pico


#include <Wire.h>


void setup() {
    Serial.begin(115200);
    delay(5000);
    //Wire.begin();
    //I2C0 channel needs Wire, I2C1 channel needs Wire1
    //Set pins before Wire.begin()
    if(!Wire.setSCL(9)){
      Serial.println("setscl error");
    }
    if(!Wire.setSDA(8)){
      Serial.println("setsda error");
    }
    Wire.begin(0x30); //begins as a slave on address 0x30
    Wire.onReceive(recv);
    Wire.onRequest(req);
}

static char buff[100];
void loop() {
  Serial.println("im alive");
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