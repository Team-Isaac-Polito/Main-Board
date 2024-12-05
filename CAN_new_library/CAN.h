#ifndef CAN_H
#define CAN_H

#include <esp32_can.h>
#include <cstdint> // For fixed-width integer types

/*
 * This library controls CAN communication between an NVIDIA Jetson and the Mainboard.
 * The board sends and receives motor setpoints and end effector control data.
 * The <cstring> header is used for memory manipulation (e.g., memcpy).
 */

class CAN {
public:
    // Constructor
    CAN(uint32_t moduleId);

    // Initialize the CAN bus
    void begin(uint32_t bitrate);

    // Send messages over CAN
    void sendMotorSetpoint(float speedL, float speedR);
    void sendEndEffectorSetpoint(int pitch, int headPitch, int headRoll);

    // Receive messages over CAN
    bool receiveMessage(CAN_frame_t& message);
    bool receiveMotorFeedback(float& speedL, float& speedR);
    bool receiveEndEffectorSetpoint(int& pitch, int& headPitch, int& headRoll);

private:
    uint32_t moduleId; // Unique module ID for the CAN node

    // Message IDs (taken from provided table)
    enum MessageID {
        MOTOR_SETPOINT_ID = 0x21,            // Motor setpoint ID
        MOTOR_FEEDBACK_ID = 0x22,            // Motor feedback ID
        END_EFFECTOR_PITCH_ID = 0x41,        // End effector pitch setpoint
        END_EFFECTOR_HEAD_PITCH_ID = 0x43,   // End effector head pitch setpoint
        END_EFFECTOR_HEAD_ROLL_ID = 0x45,    // End effector head roll setpoint
        END_EFFECTOR_PITCH_FEEDBACK_ID = 0x42,
        END_EFFECTOR_HEAD_PITCH_FEEDBACK_ID = 0x44,
        END_EFFECTOR_HEAD_ROLL_FEEDBACK_ID = 0x46
    };

    // Helper function to create CAN frames
    void createFrame(CAN_frame_t& frame, uint32_t id, uint8_t dlc, const void* data);
};

#endif // CAN_H