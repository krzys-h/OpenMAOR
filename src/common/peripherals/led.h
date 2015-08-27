#pragma once
#include "common/class_isr.h"
#include <avr/io.h>

class CPeripherals;

/**
 * \class CLed
 * \brief A LED! :D
 */
class CLed
{
protected:
    friend class CLeds;
    CLed(volatile uint8_t& ddr, volatile uint8_t& port, uint8_t bit);

public:
    void Set(bool value);
    bool Get();
    void Toggle();

protected:
    volatile uint8_t& m_port;
    uint8_t m_bit;
};

/**
 * \class CLeds
 * \brief All 4 LEDs
 */
class CLeds
{
protected:
    friend class CPeripherals;
    CLeds()
    : m_leds{
        CLed(DDRA, PORTA, 7),
        CLed(DDRC, PORTC, 3),
        CLed(DDRA, PORTA, 6),
        CLed(DDRC, PORTC, 1)
    }
    {}

public:
    CLed& operator[](uint8_t index)
    {
        return m_leds[index];
    }

    void Set(bool value);
    void Set(bool led0, bool led1, bool led2, bool led3);
    void Toggle();

protected:
    CLed m_leds[4];
};
