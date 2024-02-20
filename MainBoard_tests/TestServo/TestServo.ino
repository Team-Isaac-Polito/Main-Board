#include "DynamixelSerial.h"

DynamixelMotor sa(1);
DynamixelMotor sb(5);

void movePitch(int delta) {
  int posa = sa.readPosition() + delta;
  int posb = sb.readPosition() - delta;
  if (posa > 1023 || posb < 0) Serial.println("NOOOOOO");
  else {
    sa.moveSpeed(posa, 150);
    sb.moveSpeed(posb, 150);
  }
}

void moveYaw(int delta) {
  int posa = sa.readPosition() + delta;
  int posb = sb.readPosition() + delta;
  if (posa > 1023 || posb > 1023) Serial.println("NOOOOOO");
  else {
    sa.moveSpeed(posa, 150);
    sb.moveSpeed(posb, 150);
  }
}

void handleSerial() {
    if (Serial.available()) {
    char c = Serial.peek();
    if (c == 'a') {
      Serial.read();
      Serial.print("Position: ");
      Serial.println(sa.readPosition());
    } else if (c == 'b') {
      Serial.read();
      Serial.print("Position: ");
      Serial.println(sb.readPosition());
    } else if (c == 't') {
      Serial.read();
      int addr = Serial.parseInt();
      Serial.print("Testing address: ");
      Serial.print(addr);
      DynamixelMotor st(addr);
      Serial.print(", ping: ");
      Serial.println(st.ping());
    } else if (c == 'c') {
      Serial.read();
      int pos = Serial.parseInt();
      sa.moveSpeed(pos, 100);
    } else if (c == 'd') {
      Serial.read();
      int pos = Serial.parseInt();
      sb.moveSpeed(pos, 100);
    } else if (c == 'k') {
      Serial.read();
      int delta = Serial.parseInt();
      movePitch(delta);

    } else if (c == 'l') {
      Serial.read();
      int delta = Serial.parseInt();
      moveYaw(delta);
    }

  }
}

void niceMovement() {
  moveYaw(-250);
  delay(500);
  movePitch(-150);
  delay(500);
  moveYaw(250);
  delay(500);
  movePitch(150);
  delay(500);
}

void setup() {
  Serial.begin(115200);

  Serial1.setRX(1);
  Serial1.setTX(0);
  Dynamixel.setSerial(&Serial1);
  Dynamixel.begin(19200);

  sa.setMaxTorque(1023);
  sb.setMaxTorque(1023);
  sa.setAngleLimit(0,1023);
  sb.setAngleLimit(0,1023);
}

void loop() {
  //handleSerial();
  niceMovement();
}