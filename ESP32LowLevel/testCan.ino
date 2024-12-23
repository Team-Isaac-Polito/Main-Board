/*
   Using https://github.com/collin80/esp32_can
   which requires https://github.com/collin80/can_common
   Install both by downloading the ZIP file from GitHub.
*/

/*
This code must include SmartMotor.h (and the libraries included in it) to be completed. 
At the moment it sends fixed values and received ones do not update anything.
*/

#include "CAN.h"


CAN canBus;

void setup() {
  Serial.begin(115200);
  canBus.init();

}

void loop() {
  // receiving setpoints
  canBus.receiveMessage();
  
  // sending feedback 
  canBus.sendMotorFeedback();
  canBus.sendJointFeedback();
  canBus.sendEndEffectorFeedback();
}
