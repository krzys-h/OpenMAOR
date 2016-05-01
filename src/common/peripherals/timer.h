#pragma once
#include "common/class_isr.h"
#include <avr/io.h>
#include <avr/sleep.h>

namespace OpenMAOR
{
class CPeripherals;

namespace Peripherals
{

/**
 * \class CTimer
 * \brief Handles microcontroller timers (Timer0)
 */
class CTimer
{
private:
    friend class OpenMAOR::CPeripherals;
    CTimer()
    {
        StopTimer0();
    }

public:
    //! Sleep for a given amount of time in milliseconds (~0.1s accuracy, low power)
    static void Sleep(uint16_t delay)
    {
        m_timer0Counter = 0;
        StartTimer0(255-125);
        while (m_timer0Counter < (delay/100)*125/10)
        {
            set_sleep_mode(SLEEP_MODE_IDLE);
            sleep_enable();
            sleep_cpu();
            sleep_disable();
        }
        StopTimer0();
    }

private:
    static void StartTimer0(uint8_t initValue)
    {
        m_timer0InitValue = initValue;
        TCCR0 |= (1<<CS02) | (0<<CS01) | (1<<CS00); // preskaler 1024 => czestotliwosc 1/1024 * F_CPU
        TIMSK |= (1<<TOIE0); // włączenie przerwania od przepełnienia licznika
        TCNT0 = m_timer0InitValue; // ustawienie wartości początkowej
    }

    static void StopTimer0()
    {
        TCCR0 &= ~( (1<<CS02) | (1<<CS01) | (1<<CS00) );
        TIMSK &= ~(1<<TOIE0);
        TCNT0 = 0;
    }

// musi być public bo ISR
public:
    static volatile uint8_t m_timer0InitValue;
    static volatile uint8_t m_timer0Counter;
};

} // namespace Peripherals
} // namespace OpenMAOR
