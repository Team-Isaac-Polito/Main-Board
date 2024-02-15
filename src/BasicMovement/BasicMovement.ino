#include <esp32_can.h>
#include <Wire.h>

#define SLAVE_ADDR 48

CAN_FRAME CanMsg;

float leftSpeed, rightSpeed;

// print CAN frame
void printFrame(CAN_FRAME *message) {
   Serial.print(message->id, HEX);
   if (message->extended) {
      Serial.print(" X ");
   } else {
      Serial.print(" S ");
   }
   Serial.print(message->length, DEC);
   for (int i = 0; i < message->length; i++) {
      Serial.print(message->data.byte[i], HEX);
      Serial.print(" ");
   }
   Serial.println();
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Initializing...");

  //Initializing I2C
  Wire.begin(17, 18);

  // activate GND_sw
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

  // configure CAN
  CAN0.setCANPins((gpio_num_t)36, (gpio_num_t)35);
  CAN0.begin(125000);
  CAN0.watchFor();  // use this to filter for specific IDs

  Serial.println("Ready!");
}

int scale(int m) {
  return (m+55)*(254)/110 - 127; // -127,127
  //return (m+55)*(510)/110 - 255; // -255,255
  //return (m+55)*(2048)/110 - 1024; // -1024,1024
}

void loop() {
  // CAN read message
  if (CAN0.read(CanMsg)) {
    Serial.println("ESP32 received message");
    printFrame(&CanMsg);

    if ((CanMsg.id & 0x00FF0000)>>16 == 0x21) {
       Serial.println("MOVEMENT MESSAGE");

      // take the first four bytes from the array and and put them in a float
      memcpy(&leftSpeed, &(CanMsg.data), 4);
      memcpy(&rightSpeed, &(CanMsg.data), 4);
      // motorTrLeft.setSpeed(leftSpeed);
      // motorTrRight.setSpeed(rightSpeed);

      //I2C Send data to motors
      int left = scale((int)leftSpeed);
      int right = scale((int)rightSpeed);
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write(left);
      Wire.write(right);
      byte error = Wire.endTransmission();
      Serial.print("Result: ");
      Serial.println(error);
      Serial.println("Receive data");

      Serial.print("TRACTION DATA :\tleft: \t");
      Serial.print(leftSpeed);
      Serial.print("\tright: \t");
      Serial.println(rightSpeed);
    }
  }

  delay(5);
}
