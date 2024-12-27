#include "CAN.h"

#include <cstring>

#define CAN_ID    0x11  // this shouldn't be here in the end but specified in the makefile


// 
CAN::CAN() {}

// I
void CAN::init() {
// configure CAN
   CAN0.setCANPins((gpio_num_t)36, (gpio_num_t)35);
   CAN0.begin(125000);
   CAN0.watchFor(); // use this to filter for specific IDs
 
 // DEBUG
  Serial.println("Can init...");
}



// send motor feedback
void CAN::sendMotorFeedback() {
    canMsg.id = 0; // Reset 
    canMsg.id = CAN_ID | (MOTOR_FEEDBACK_ID << 16);     
    canMsg.length = 8;  
    canMsg.rtr = 0;    
    canMsg.extended = 1; 
     // send motor feedback as float

  // in the end, uncomment these two lines and delete the two test values  
  //float speedR = motorTrRight.getSpeed();
  //float speedL = motorTrLeft.getSpeed();
  float speedR = 2.0;
  float speedL = 3.5;
  memcpy(&canMsg.data.uint8[0], &speedL, 4); 
  memcpy(&canMsg.data.uint8[4], &speedR, 4);
 
  CAN0.sendFrame(canMsg);
}


void CAN::sendJointFeedback() {
// in the end, uncomment these two lines and delete the test value
  //encoderYaw.update();
  //float angle = encoderYaw.readAngle();
  float angle = 7;
  canMsg.id = 0; 
  canMsg.id = CAN_ID | (JOINT_YAW_FEEDBACK_ID << 16);     
  canMsg.length = 4;  
  canMsg.rtr = 0;    
  canMsg.extended = 1; 
  memcpy(canMsg.data.uint32, &angle, 4);
  CAN0.sendFrame(canMsg);

}



// send end effector's feedback
void CAN::sendEndEffectorFeedback() {
  // in the end, uncomment this line and delete the test value
// int pitch = motorEEPitch.readPosition();
  int pitch = 1;
  canMsg.id = 0; // Reset 
  canMsg.id = CAN_ID | (END_EFFECTOR_PITCH_FEEDBACK_ID << 16);     
  canMsg.length = 4;  
  canMsg.rtr = 0;    
  canMsg.extended = 1; 
  memcpy(canMsg.data.uint8, &pitch, 4);
  CAN0.sendFrame(canMsg);

  // in the end, uncomment this line and delete the test value
  //int headPitch = motorEEHeadPitch.readPosition();
  int headPitch = 1;

  canMsg.id = CAN_ID | (END_EFFECTOR_HEAD_PITCH_FEEDBACK_ID << 16);     
  canMsg.length = 4;  
  canMsg.rtr = 0;    
  canMsg.extended = 1; 
  memcpy(canMsg.data.uint8, &headPitch, 4);
  CAN0.sendFrame(canMsg);

  // in the end, uncomment this line and delete the test value
  //int headRoll = motorEEHeadRoll.readPosition();
  int headRoll = 1;
  canMsg.id = CAN_ID | (END_EFFECTOR_HEAD_ROLL_FEEDBACK_ID << 16);     
  canMsg.length = 4;  
  canMsg.rtr = 0;    
  canMsg.extended = 1; 
  memcpy(canMsg.data.uint8, &headRoll, 4);
  CAN0.sendFrame(canMsg);

}

bool CAN::receiveMotorSetpoint() {
    // take the first four bytes from the array and and put them in a float
        float leftSpeed, rightSpeed;
        memcpy(&leftSpeed, canMsg.data.uint8, 4); // Copia i primi 4 byte per leftSpeed
        memcpy(&rightSpeed, canMsg.data.uint8 + 4, 4);
      // in the end, uncomment these two lines
        //motorTrLeft.setSpeed(leftSpeed);
        //motorTrRight.setSpeed(rightSpeed);
        }
  




bool CAN::receiveJointSetpoint() {
  float yawA;
  memcpy(&yawA, canMsg.data.uint8, 4);
  // to be completed
  }
      


// receive end effector setpoint
bool CAN::receiveEndEffectorSetpoint(byte type) {
   int16_t data;

      switch (type) {
        case END_EFFECTOR_PITCH_SETPOINT_ID:
        // take the first four bytes from the array and and put them in a float
        memcpy(&data, canMsg.data.uint8, 4);
      // in the end, uncomment this line
        //motorEEPitch.moveSpeed(data, SERVO_SPEED);        
        break;
         break;
        case END_EFFECTOR_HEAD_PITCH_SETPOINT_ID:
        // take the first four bytes from the array and and put them in a float
        memcpy(&data, canMsg.data.uint8, 4);
        // in the end, uncomment this line
        //motorEEPitch.moveSpeed(data, SERVO_SPEED);
                
        break;
        case END_EFFECTOR_HEAD_ROLL_SETPOINT_ID:
        // take the first four bytes from the array and and put them in a float
        memcpy(&data, canMsg.data.uint8, 4);
        // in the end, uncomment this line
        //motorEEPitch.moveSpeed(data, SERVO_SPEED);
                
        break;  
}
}

bool CAN::receiveMessage() {
  unsigned long time_cur = millis(); // current time
  if (CAN0.read(canMsg)) {
    time_data = time_cur;
    byte type = canMsg.id >> 16;


    // DEBUG: print frame
      Serial.println("I'm receiving...");
          Serial.print(canMsg.id, HEX);
          if (canMsg.extended) {
            Serial.print(" X ");}
          else {
            Serial.print(" S ");  } 
            Serial.print(canMsg.length, DEC);
            for (int i = 0; i < canMsg.length; i++) {
            Serial.print(canMsg.data.byte[i], HEX);
            Serial.print(" ");}
            Serial.println();

    
    if (type == MOTOR_SETPOINT_ID) {
      receiveMotorSetpoint();
    } 
    else if (type == END_EFFECTOR_PITCH_SETPOINT_ID || 
             type == END_EFFECTOR_HEAD_PITCH_SETPOINT_ID || 
             type == END_EFFECTOR_HEAD_ROLL_SETPOINT_ID) {
      receiveEndEffectorSetpoint(type);
    } 
    else if (type == JOINT_YAW_SETPOINT_ID) {
      receiveJointSetpoint();
    }

    return true;
  }
  // Timeout
  else if (time_cur - time_data > TIMEOUT && time_data != 0) {
    time_data = -1; 

    //DEBUG
     Serial.println("Timeout reached.");
    // in the end, uncomment these two lines
    // motorTrLeft.stop();
    // motorTrRight.stop();
  }

  return false; // No msg received
}
