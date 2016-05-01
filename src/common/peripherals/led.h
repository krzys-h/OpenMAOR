#pragma once
#include "common/class_isr.h"
#include <avr/io.h>

namespace OpenMAOR
{
class CPeripherals;

namespace Peripherals
{

/**
 * \class CLed
 * \brief A LED! :D
 */
class CLed
{
private:
    friend class CLeds;
    CLed(volatile uint8_t& ddr, volatile uint8_t& port, uint8_t bit)
        : m_port(port)
        , m_bit(bit)
    {
        ddr |= _BV(m_bit);
    }

public:
    void Set(bool value)
    {
        if (value)
        {
            m_port |= _BV(m_bit);
        }
        else
        {
            m_port &= ~(_BV(m_bit));
        }
    }

    bool Get()
    {
        return (m_port & _BV(m_bit)) != 0;
    }

    void Toggle()
    {
        m_port ^= _BV(m_bit);
    }

private:
    volatile uint8_t& m_port;
    uint8_t m_bit;
};

/**
 * \class CLeds
 * \brief All 4 LEDs
 */
class CLeds
{
private:
    friend class OpenMAOR::CPeripherals;
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

    void Set(bool value)
    {
        for (uint8_t i = 0; i < 4; i++)
            m_leds[i].Set(value);
    }

    void Set(bool led0, bool led1, bool led2, bool led3)
    {
        m_leds[0].Set(led0);
        m_leds[1].Set(led1);
        m_leds[2].Set(led2);
        m_leds[3].Set(led3);
    }

    void Toggle()
    {
        for (uint8_t i = 0; i < 4; i++)
            m_leds[i].Toggle();
    }

private:
    CLed m_leds[4];
};

} // namespace Peripherals
} // namespace OpenMAOR
