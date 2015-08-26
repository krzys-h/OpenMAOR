#include "peripherals/led.h"
#include <avr/io.h>

CLed::CLed(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit)
{
    m_port = port;
    m_bit = bit;

    *ddr |= _BV(m_bit);
}

void CLed::Set(bool value)
{
    if (value)
    {
        *m_port |= _BV(m_bit);
    }
    else
    {
        *m_port &= ~(_BV(m_bit));
    }
}

bool CLed::Get()
{
    return (*m_port & _BV(m_bit)) != 0;
}

void CLed::Toggle()
{
    *m_port ^= _BV(m_bit);
}
