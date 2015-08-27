#pragma once
#include "common/singleton.h"
#include "common/class_isr.h"
#include <avr/io.h>

class CPeripherals;

/**
 * \class CTimer
 * \brief Handles microcontroller timers
 */
// singleton dla ISR
class CTimer : public CSingleton<CTimer>
{
protected:
    friend class CPeripherals;
    CTimer();

public:
    //! Sleep for a given amount of time in milliseconds (~0.1s accuracy, low power)
    void Sleep(uint16_t delay);

    DECLARE_CLASS_ISR(TIMER0_OVF_vect);

//protected:
    void StartTimer0(uint8_t initValue);
    void StopTimer0();

    volatile uint8_t m_timer0InitValue;
    volatile uint8_t m_timer0Counter;
};
