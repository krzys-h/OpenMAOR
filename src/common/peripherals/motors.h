#pragma once
#include "common/class_isr.h"
#include <avr/io.h>

#include "common/extra_section.h"

namespace OpenMAOR
{
class CPeripherals;

namespace Peripherals
{

/**
 * \class CMotors
 * \brief Controls the motors
 */
class CMotors
{
private:
    friend class OpenMAOR::CPeripherals;
    CMotors();

public:
    static const uint16_t MAX_SPEED = 1023;
    static void SetLeft(int16_t value) EXTRA;
    static void SetRight(int16_t value) EXTRA;
    inline static void SetMotors(int16_t left, int16_t right)
    {
        SetLeft(left);
        SetRight(right);
    }
    inline static void SetMotors(int16_t value)
    {
        SetLeft(value);
        SetRight(value);
    }
    inline static void Stop()
    {
        SetMotors(0);
    }
    inline static int16_t GetLeft()
    {
        return m_speedLeft;
    }
    inline static int16_t GetRight()
    {
        return m_speedRight;
    }

    // TODO: Doesn't this overflow? :/
    inline static int16_t PercentageToSpeed(int8_t percentage)
    {
        return percentage * MAX_SPEED / 100;
    }
    inline static int8_t SpeedToPercentage(int16_t speed)
    {
        return speed * 100 / MAX_SPEED;
    }

private:
    static int16_t m_speedLeft;
    static int16_t m_speedRight;

private:
    inline static void StartPWM_A() { TCCR1A |= (1<<COM1A1); }
    inline static void SetPWM_A(uint16_t value) { OCR1A = value; }
    inline static void StopPWM_A() { TCCR1A &= ~(1<<COM1A1); }

    inline static void StartPWM_B() { TCCR1B |= (1<<COM1B1); }
    inline static void SetPWM_B(uint16_t value) { OCR1B = value; }
    inline static void StopPWM_B() { TCCR1B &= ~(1<<COM1B1); }
};

} // namespace Peripherals
} // namespace OpenMAOR
