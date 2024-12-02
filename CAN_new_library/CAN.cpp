#include "CAN.h"
#include <cstring>

// Costruttore
CAN::CAN(uint32_t moduleId) : moduleId(moduleId) {}

// Inizializzazione del bus CAN
void CAN::begin(uint32_t bitrate) {
    CAN_init(); // Inizializza l'hardware CAN
    CAN_configure(bitrate, CAN_MODE_NORMAL); // Configura bitrate e modalità normale
    CAN_start(); // Avvia il bus CAN
}

// Funzione helper per creare frame CAN
void CAN::createFrame(CAN_frame_t& frame, uint32_t id, uint8_t dlc, const void* data) {
    frame.FIR.B.FF = CAN_frame_ext; // Frame esteso
    frame.MsgID = (moduleId << 8) | (id << 16); // ID con modulo e identificatore
    frame.FIR.B.DLC = dlc; // Lunghezza dati
    memcpy(frame.data.u8, data, dlc); // Copia i dati nel frame
}

// Invio di feedback del motore
void CAN::sendMotorFeedback(float speedL, float speedR) {
    CAN_frame_t frame;
    float data[2] = {speedL, speedR};
    createFrame(frame, MOTOR_SETPOINT_ID, 8, data);
    CAN_write(&frame);
}

// Invio di setpoint del motore
void CAN::sendMotorSetpoint(float speedL, float speedR) {
    CAN_frame_t frame;
    float data[2] = {speedL, speedR};
    createFrame(frame, MOTOR_SETPOINT_ID, 8, data);
    CAN_write(&frame);
}

// Invio di setpoint dell'end effector
void CAN::sendEndEffectorSetpoint(int pitch, int headPitch, int headRoll) {
    CAN_frame_t frame;

    // Invia pitch
    createFrame(frame, END_EFFECTOR_PITCH_ID, 4, &pitch);
    CAN_write(&frame);

    // Invia headPitch
    createFrame(frame, END_EFFECTOR_HEAD_PITCH_ID, 4, &headPitch);
    CAN_write(&frame);

    // Invia headRoll
    createFrame(frame, END_EFFECTOR_HEAD_ROLL_ID, 4, &headRoll);
    CAN_write(&frame);
}

// Ricezione generica di un messaggio CAN
bool CAN::receiveMessage(CAN_frame_t& message) {
    if (CAN_rx_available()) {
        CAN_read(&message); // Legge il messaggio dal buffer
        return true;
    }
    return false;
}

// Ricezione di feedback del motore
bool CAN::receiveMotorFeedback(float& speedL, float& speedR) {
    CAN_frame_t message;
    if (receiveMessage(message) && (message.MsgID >> 16 & 0xFF) == MOTOR_SETPOINT_ID) {
        memcpy(&speedL, &message.data.u8[0], 4);
        memcpy(&speedR, &message.data.u8[4], 4);
        return true;
    }
    return false;
}

// Ricezione di setpoint dell'end effector
bool CAN::receiveEndEffectorSetpoint(int& pitch, int& headPitch, int& headRoll) {
    CAN_frame_t message;

    // Ricezione pitch
    if (receiveMessage(message) && (message.MsgID >> 16 & 0xFF) == END_EFFECTOR_PITCH_ID) {
        memcpy(&pitch, &message.data.u8[0], 4);
    }

    // Ricezione headPitch
    if (receiveMessage(message) && (message.MsgID >> 16 & 0xFF) == END_EFFECTOR_HEAD_PITCH_ID) {
        memcpy(&headPitch, &message.data.u8[0], 4);
    }

    // Ricezione headRoll
    if (receiveMessage(message) && (message.MsgID >> 16 & 0xFF) == END_EFFECTOR_HEAD_ROLL_ID) {
        memcpy(&headRoll, &message.data.u8[0], 4);
        return true;
    }
    return false;
}
