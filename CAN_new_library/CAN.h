/* 
   Using https://github.com/collin80/esp32_can
   which requires https://github.com/collin80/can_common
   Install both by downloading the ZIP file from GitHub.
*/

#ifndef CAN_H
#define CAN_H

#include <esp32_can.h>

class CAN {
public:
    // Costruttore
    CAN(uint32_t moduleId);

    // Inizializzazione del bus CAN
    void begin(uint32_t bitrate);

    // Invio di messaggi CAN
    void sendMotorFeedback(float speedL, float speedR);
    void sendMotorSetpoint(float speedL, float speedR);
    void sendEndEffectorSetpoint(int pitch, int headPitch, int headRoll);

    // Ricezione di messaggi CAN
    bool receiveMessage(CAN_frame_t& message);
    bool receiveMotorFeedback(float& speedL, float& speedR);
    bool receiveEndEffectorSetpoint(int& pitch, int& headPitch, int& headRoll);

private:
    uint32_t moduleId; // ID univoco del modulo

    // Enumerazioni per gli identificatori
    enum MessageID {
        MOTOR_SETPOINT_ID = 0x21,
        END_EFFECTOR_PITCH_ID = 0x41,
        END_EFFECTOR_HEAD_PITCH_ID = 0x43,
        END_EFFECTOR_HEAD_ROLL_ID = 0x45
    };

    // Funzione helper per creare messaggi
    void createFrame(CAN_frame_t& frame, uint32_t id, uint8_t dlc, const void* data);
};

#endif // CAN_H
