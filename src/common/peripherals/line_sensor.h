#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

namespace OpenMAOR
{
class CPeripherals;

namespace Peripherals
{
// TODO: This is untested!

/**
 * \class CLineSensor
 * \brief A line sensor
 */
class CLineSensor
{
private:
    friend class CLineSensors;
    CLineSensor(volatile uint8_t& ddr, volatile uint8_t& port, volatile uint8_t& pin, uint8_t bit)
        : m_pin(pin)
        , m_bit(bit)
    {
        ddr &= ~(_BV(m_bit));
        port &= ~(_BV(m_bit));
    }

public:
    bool Get()
    {
        return (m_pin & _BV(m_bit)) != 0;
    }

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
    CLineSensors()
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
    CLineSensor& operator[](uint8_t index)
    {
        return m_lineSensors[index];
    }

    void Enable(bool enable)
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
