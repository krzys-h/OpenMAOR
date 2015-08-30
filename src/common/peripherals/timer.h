#pragma once
#include "common/class_isr.h"
#include <avr/io.h>

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
    CTimer();

public:
    //! Sleep for a given amount of time in milliseconds (~0.1s accuracy, low power)
    static void Sleep(uint16_t delay);

private:
    static void StartTimer0(uint8_t initValue);
    static void StopTimer0();

// musi być public bo ISR
public:
    static volatile uint8_t m_timer0InitValue;
    static volatile uint8_t m_timer0Counter;
};

} // namespace Peripherals
} // namespace OpenMAOR
