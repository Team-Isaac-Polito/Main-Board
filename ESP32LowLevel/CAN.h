/* 
   Using https://github.com/collin80/esp32_can
   which requires https://github.com/collin80/can_common
   Install both by downloading the ZIP file from GitHub.

*/

#include <esp32_can.h>
#include "mod_config.h"
#ifndef CAN_H
#define CAN_H




class CAN {
public:
    // Constructor
    CAN();

    // Configure CAN
    void init();

    // send messages
    void sendMotorFeedback();
    void sendJointFeedback();
    void sendEndEffectorFeedback();

    // receive messages
    bool receiveMessage();    
    bool receiveMotorSetpoint();
    bool receiveJointSetpoint();
    bool receiveEndEffectorSetpoint(byte type);

private:
    CAN_FRAME canMsg;
    unsigned long time_data = -1;
    const unsigned long TIMEOUT = 1000; 

    // this enum cointains all message IDs (to be sent and to be received)
    enum MessageID {
        MOTOR_SETPOINT_ID = 0x21,
        MOTOR_FEEDBACK_ID = 0x22,
        JOINT_YAW_SETPOINT_ID = 0x31,
        JOINT_YAW_FEEDBACK_ID = 0x32,
        //JOINT_PITCH_SETPOINT_ID = 0x33,
        //JOINT_PITCH_FEEDBACK_ID = 0x34,
        //JOINT_ROLL_SETPOINT_ID = 0x35,
        //JOINT_ROLL_FEEDBACK_ID = 0x36,
        END_EFFECTOR_PITCH_SETPOINT_ID = 0x41,
        END_EFFECTOR_PITCH_FEEDBACK_ID = 0x42,
        END_EFFECTOR_HEAD_PITCH_SETPOINT_ID = 0x43,
        END_EFFECTOR_HEAD_PITCH_FEEDBACK_ID = 0x44,
        END_EFFECTOR_HEAD_ROLL_SETPOINT_ID = 0x45,
        END_EFFECTOR_HEAD_ROLL_FEEDBACK_ID = 0x46,
        
    };


};

#endif // CAN_H