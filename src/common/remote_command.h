#pragma once

/**
 * \class CPeripheralRemoteCommandExecutor
 * \brief Default executor for peripheral commands
 *
 * TODO: Implement this
 */
class CPeripheralRemoteCommandExecutor {
public:
    float GetLeftEngine() { return 0.0f; };
    void SetLeftEngine(float power) {};
    uint8_t GetLeftEnginePower() { return 0; };

    float GetRightEngine() { return 0.0f; };
    void SetRightEngine(float power) {};
    uint8_t GetRightEnginePower() { return 0; };

    bool GetLED(int led) { return false; };
    void SetLED(int led, bool status) {};

    uint8_t GetBatteryStatus() { return 0xFF; };

    bool GetLineSensor(int id) { return false; };

    uint8_t GetLeftSonar() { return 0; };
    uint8_t GetRightSonar() { return 0; };

    void StartProgram() {};
    void StopProgram() {};

    uint16_t GetCustomADC(int id) { return 0; }; // 10-bit
};

/**
 * \class CRemoteCommandExecutor
 * \brief Interface for class that executes remote control commands
 */
class CRemoteCommandExecutor : public CPeripheralRemoteCommandExecutor {};
