#pragma once
#include "common/class_isr.h"
#include <avr/io.h>

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
    CMotors()
    {
        //TODO: sprawdzić
        TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM10)|(1<<WGM11);
        // Tryb 10-bit Fast PWM inverting mode,
        // clk/64, co daje okres ok 4ms
        TCCR1B|=(1<<WGM12)|(1<<CS11)|(1<<CS10);
        //TCCR1B|=(1<<WGM12)|(1<<CS12);

        // Wyjscia PWM
        DDRD |= _BV(5); // PWMA
        DDRD |= _BV(4); // PWMB

        // lewy
        DDRC |= _BV(6);
        DDRC |= _BV(5);

        // prawy
        DDRC |= _BV(2);
        DDRC |= _BV(4);
    }

public:
    static const uint16_t MAX_SPEED = 1023;

    static void SetLeft(int16_t value)
    {
        if (value > MAX_SPEED) value = MAX_SPEED;
        if (value < -MAX_SPEED) value = -MAX_SPEED;

        if (value > 0)
        {
            // do przodu - A=1, B=0
            PORTC |= _BV(6);
            PORTC &= ~(_BV(5));
            StartPWM_B();
            SetPWM_B(value);
        }
        else if (value < 0)
        {
            // do tylu - A=0, B=1
            PORTC &= ~(_BV(6));
            PORTC |= _BV(5);
            StartPWM_B();
            SetPWM_B(-value);
        }
        else
        {
            // STOP - A=0, B=0
            PORTC &= ~(_BV(6));
            PORTC &= ~(_BV(5));
            StopPWM_B();
            SetPWM_B(0);
        }

        m_speedLeft = value;
    }

    static void SetRight(int16_t value)
    {
        if (value > MAX_SPEED) value = MAX_SPEED;
        if (value < -MAX_SPEED) value = -MAX_SPEED;

        if (value > 0)
        {
            // do przodu - A=1, B=0
            PORTC |= _BV(2);
            PORTC &= ~(_BV(4));
            StartPWM_A();
            SetPWM_A(value);
        }
        else if (value < 0)
        {
            // do tylu - A=0, B=1
            PORTC &= ~(_BV(2));
            PORTC |= _BV(4);
            StartPWM_A();
            SetPWM_A(-value);
        }
        else
        {
            // STOP - A=0, B=0
            PORTC &= ~(_BV(2));
            PORTC &= ~(_BV(4));
            StopPWM_A();
            SetPWM_A(0);
        }

        m_speedRight = value;
    }

    static void SetMotors(int16_t left, int16_t right)
    {
        SetLeft(left);
        SetRight(right);
    }
    static void SetMotors(int16_t value)
    {
        SetLeft(value);
        SetRight(value);
    }
    static void Stop()
    {
        SetMotors(0);
    }
    static int16_t GetLeft()
    {
        return m_speedLeft;
    }
    static int16_t GetRight()
    {
        return m_speedRight;
    }

    // TODO: Doesn't this overflow? :/
    static int16_t PercentageToSpeed(int8_t percentage)
    {
        return percentage * MAX_SPEED / 100;
    }
    static int8_t SpeedToPercentage(int16_t speed)
    {
        return speed * 100 / MAX_SPEED;
    }

private:
    static int16_t m_speedLeft;
    static int16_t m_speedRight;

private:
    static void StartPWM_A() { TCCR1A |= (1<<COM1A1); }
    static void SetPWM_A(uint16_t value) { OCR1A = value; }
    static void StopPWM_A() { TCCR1A &= ~(1<<COM1A1); }

    static void StartPWM_B() { TCCR1B |= (1<<COM1B1); }
    static void SetPWM_B(uint16_t value) { OCR1B = value; }
    static void StopPWM_B() { TCCR1B &= ~(1<<COM1B1); }
};

} // namespace Peripherals
} // namespace OpenMAOR
