#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "common/extra_section.h"

namespace OpenMAOR
{
class CPeripherals;

namespace Peripherals
{

/**
 * \class CLineSensor
 * \brief A line sensor
 */
class CLineSensor
{
private:
    friend class CLineSensors;
    CLineSensor(volatile uint8_t& ddr, volatile uint8_t& port, volatile uint8_t& pin, uint8_t bit) EXTRA;

public:
    bool Get() EXTRA;

private:
    volatile uint8_t& m_pin;
    uint8_t m_bit;
};

/**
 * \class CLineSensors
 * \brief All 4 line sensors
 */
class CLineSensors
{
private:
    friend class OpenMAOR::CPeripherals;
    CLineSensors() EXTRA
    : m_lineSensors{
        CLineSensor(DDRB, PORTB, PINB, 1),
        CLineSensor(DDRD, PORTD, PIND, 7),
        CLineSensor(DDRB, PORTB, PINB, 0),
        CLineSensor(DDRC, PORTC, PINC, 0)
    }
    {
        DDRD |= _BV(6);
    }

public:
    CLineSensor& operator[](uint8_t index) EXTRA
    {
        return m_lineSensors[index];
    }

    void Enable(bool enable) EXTRA
    {
        if (enable)
        {
            PORTD |= _BV(6);
            _delay_us(50);
        }
        else
        {
            PORTD &= ~(_BV(6));
        }
    }

private:
    CLineSensor m_lineSensors[4];
};

} // namespace Peripherals
} // namespace OpenMAOR
