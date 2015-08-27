#pragma once

#include "peripherals/peripherals.h"

/**
 * \class CRemoteCommandExecutor
 * \brief Interface for class that executes remote control commands
 *
 * TODO: Implement this
 */
class CRemoteCommandExecutor {
public:
    CRemoteCommandExecutor(CPeripherals* robot) : m_robot(robot) {};

    float GetLeftEngine() { return 0.0f; };
    void SetLeftEngine(float power) {};
    uint8_t GetLeftEnginePower() { return 0; };

    float GetRightEngine() { return 0.0f; };
    void SetRightEngine(float power) {};
    uint8_t GetRightEnginePower() { return 0; };

    bool GetLED(uint8_t led) { return m_robot->led[led].Get(); };
    void SetLED(uint8_t led, bool value) { m_robot->led[led].Set(value); };

    uint8_t GetBatteryStatus() { return 0xFF; };

    bool GetLineSensor(uint8_t id) { return false; };

    uint8_t GetLeftSonar() { return 0; };
    uint8_t GetRightSonar() { return 0; };

    void StartProgram() {};
    void StopProgram() {};

    uint16_t GetCustomADC(uint8_t id) { return 0; }; // 10-bit

protected:
    CPeripherals* m_robot;
};
