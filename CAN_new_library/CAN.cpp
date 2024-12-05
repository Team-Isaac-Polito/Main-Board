#include "CAN.h"
#include <cstring> // Used for memcpy in data transmission

// Constructor: Initialize the CAN object with a specific module ID
CAN::CAN(uint32_t moduleId) : moduleId(moduleId) {}

// Initialize the CAN bus with a specified bitrate
void CAN::begin(uint32_t bitrate) {
    CAN_init(); // Initialize the CAN hardware
    CAN_configure(bitrate, CAN_MODE_NORMAL); // Configure bitrate and normal mode
    CAN_start(); // Start the CAN bus
}

// Helper function to create a CAN frame
void CAN::createFrame(CAN_frame_t& frame, uint32_t id, uint8_t dlc, const void* data) {
    frame.FIR.B.FF = CAN_frame_ext; // Extended frame format
    frame.MsgID = (moduleId << 8) | (id << 16); // Combine module ID and message ID
    frame.FIR.B.DLC = dlc; // Data length code (DLC)
    memcpy(frame.data.u8, data, dlc); // Copy data into the frame
}

// Send motor setpoints (two floats representing left and right motor speeds)
void CAN::sendMotorSetpoint(float speedL, float speedR) {
    CAN_frame_t frame;
    float data[2] = {speedL, speedR}; // Left and right motor speeds
    createFrame(frame, MOTOR_SETPOINT_ID, 8, data); // Create frame with 8 bytes
    CAN_write(&frame); // Send the frame
}

// Send setpoints for the end effector's pitch, head pitch, and head roll (as integers)
void CAN::sendEndEffectorSetpoint(int pitch, int headPitch, int headRoll) {
    CAN_frame_t frame;

    // Send pitch setpoint
    createFrame(frame, END_EFFECTOR_PITCH_ID, 4, &pitch);
    CAN_write(&frame);

    // Send head pitch setpoint
    createFrame(frame, END_EFFECTOR_HEAD_PITCH_ID, 4, &headPitch);
    CAN_write(&frame);

    // Send head roll setpoint
    createFrame(frame, END_EFFECTOR_HEAD_ROLL_ID, 4, &headRoll);
    CAN_write(&frame);
}

// Receive a general CAN message
bool CAN::receiveMessage(CAN_frame_t& message) {
    if (CAN_rx_available()) { // Check if a message is available
        CAN_read(&message); // Read the message from the buffer
        return true;
    }
    return false;
}

// Receive motor feedback (left and right motor speeds)
bool CAN::receiveMotorFeedback(float& speedL, float& speedR) {
    CAN_frame_t message;
    if (receiveMessage(message) && (message.MsgID >> 16 & 0xFF) == MOTOR_FEEDBACK_ID) {
        memcpy(&speedL, &message.data.u8[0], 4);
        memcpy(&speedR, &message.data.u8[4], 4);
        return true;
    }
    return false;
}

// Receive end effector setpoints for pitch, head pitch, and head roll
bool CAN::receiveEndEffectorSetpoint(int& pitch, int& headPitch, int& headRoll) {
    CAN_frame_t message;

    // Receive pitch
    if (receiveMessage(message) && (message.MsgID >> 16 & 0xFF) == END_EFFECTOR_PITCH_FEEDBACK_ID) {
        memcpy(&pitch, &message.data.u8[0], 4);
    }

    // Receive head pitch
    if (receiveMessage(message) && (message.MsgID >> 16 & 0xFF) == END_EFFECTOR_HEAD_PITCH_FEEDBACK_ID) {
        memcpy(&headPitch, &message.data.u8[0], 4);
    }

    // Receive head roll
    if (receiveMessage(message) && (message.MsgID >> 16 & 0xFF) == END_EFFECTOR_HEAD_ROLL_FEEDBACK_ID) {
        memcpy(&headRoll, &message.data.u8[0], 4);
        return true;
    }
    return false;
}