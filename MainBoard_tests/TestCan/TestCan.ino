/* 
   Using https://github.com/collin80/esp32_can
   which requires https://github.com/collin80/can_common
   Install both by downloading the ZIP file from GitHub.
*/
#include <esp32_can.h>

CAN_FRAME txFrame, message;

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
   while (!Serial);
   Serial.println("Initializing...");

   // activate GND_sw
   pinMode(8, OUTPUT);
   digitalWrite(8, HIGH);

   // configure CAN
   CAN0.setCANPins((gpio_num_t)36, (gpio_num_t)35);
   CAN0.begin(125000);
   CAN0.watchFor(); // use this to filter for specific IDs

   Serial.println("Ready!");

   txFrame.rtr = 0;
   txFrame.id = 0x123;
   txFrame.extended = false;
   txFrame.length = 4;
   txFrame.data.uint8[0] = 0x10;
   txFrame.data.uint8[1] = 0x1A;
   txFrame.data.uint8[2] = 0xFF;
   txFrame.data.uint8[3] = 0x5D;
}

void loop() {
   delay(500);

   // send package
   CAN0.sendFrame(txFrame);
   Serial.println("ESP32 sent message");
   printFrame(&txFrame);

   // read message
   if (CAN0.read(message)) {
      Serial.println("ESP32 received message");
      printFrame(&message);
   }
}

