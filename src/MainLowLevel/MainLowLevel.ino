#include <esp32_can.h>
#include <Wire.h>


#define SLAVE_ADDR 48

CAN_FRAME CanMsg;

float leftSpeed, rightSpeed;

// print CAN frame
void printFrame(CAN_FRAME *message)
{
  Serial.print(message->id, HEX);
  if (message->extended)
  {
    Serial.print(" X ");
  }
  else
  {
    Serial.print(" S ");
  }
  Serial.print(message->length, DEC);
  Serial.print(" ");
  for (int i = 0; i < message->length; i++)
  {
    Serial.print(message->data.byte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Initializing...");

  // Initializing I2C
  Wire.begin(17, 18);

  // activate GND_sw
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

  // configure CAN
  CAN0.setCANPins((gpio_num_t)36, (gpio_num_t)35);
  CAN0.begin(125000);
  CAN0.watchFor(); // use this to filter for specific IDs

  Serial.println("Ready!");
}

short int scale(int m)
{
  // return (m + 55) * (254) / 110 - 127; // -127,127
  // return (m+55)*(510)/110 - 255; // -255,255
  return (m + 55) * (2048) / 110 - 1024; // -1024,1024
}

// short int is 16bits or 2bytes, enough to represent -1024,1024
void sendIntI2C(short int value)
{
  unsigned char *ptr = (unsigned char *)&value;

  Wire.write(*(ptr + 1)); // Send MSB first so they can be shifted left
  Wire.write(*(ptr + 0));
}

void printI2C_ecode(byte exitcode)
{
  Serial.print("I2C exit code: ");
  switch (exitcode)
  {
  case 0:
    Serial.println("success");
    break;
  case 1:
    Serial.println("data too long to fit in transmit buffer");
    break;
  case 2:
    Serial.println("received NACK on transmit of address.");
    break;
  case 3:
    Serial.println("received NACK on transmit of data.");
    break;
  case 4:
    Serial.println("other error.");
    break;
  case 5:
    Serial.println("timeout");
    break;
  default:
    Serial.println("unexpected error");
    break;
  }
}

void loop()
{
  // CAN read message
  if (CAN0.read(CanMsg))
  {
    printFrame(&CanMsg);

    if ((CanMsg.id & 0x00FF0000) >> 16 == 0x21)
    {
      // take the first four bytes from the array and and put them in a float
      memcpy(&leftSpeed, &(CanMsg.data), 4);
      memcpy(&rightSpeed, &(CanMsg.data.byte[4]) , 4);

      // I2C Send data to motors
      Wire.beginTransmission(SLAVE_ADDR);
      sendIntI2C((short int)scale((int)leftSpeed));
      sendIntI2C((short int)scale((int)rightSpeed));
      Wire.endTransmission();

      Serial.print("TRACTION DATA :\tleft: \t");
      Serial.print(leftSpeed);
      Serial.print("\tright: \t");
      Serial.println(rightSpeed);
    }
  }
  else
  {
    Serial.println("No CAN received");
  }

  delay(5);
}
