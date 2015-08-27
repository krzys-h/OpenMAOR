#include "common/peripherals/led.h"
#include <avr/io.h>

CLed::CLed(volatile uint8_t& ddr, volatile uint8_t& port, uint8_t bit)
    : m_port(port)
    , m_bit(bit)
{
    ddr |= _BV(m_bit);
}

void CLed::Set(bool value)
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

bool CLed::Get()
{
    return (m_port & _BV(m_bit)) != 0;
}

void CLed::Toggle()
{
    m_port ^= _BV(m_bit);
}

// =============================================================================

void CLeds::Set(bool value)
{
    for (uint8_t i = 0; i < 4; i++)
        m_leds[i].Set(value);
}

void CLeds::Set(bool led0, bool led1, bool led2, bool led3)
{
    m_leds[0].Set(led0);
    m_leds[1].Set(led1);
    m_leds[2].Set(led2);
    m_leds[3].Set(led3);
}

void CLeds::Toggle()
{
    for (uint8_t i = 0; i < 4; i++)
        m_leds[i].Toggle();
}
