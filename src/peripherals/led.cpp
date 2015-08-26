#include "peripherals/led.h"
#include <avr/io.h>

CLed::CLed(int id)
{
    m_id = id;

    switch (m_id)
    {
        case 0:
            DDRA |= _BV(7);
            break;

        case 1:
            DDRC |= _BV(3);
            break;

        case 2:
            DDRA |= _BV(6);
            break;

        case 3:
            DDRC |= _BV(1);
            break;

        default:
            assert(false);
    }
}

void CLed::Set(bool value)
{
    if (value)
    {
        switch (m_id)
        {
            case 0:
                PORTA |= _BV(7);
                break;

            case 1:
                PORTC |= _BV(3);
                break;

            case 2:
                PORTA |= _BV(6);
                break;

            case 3:
                PORTC |= _BV(1);
                break;

            default:
                assert(false);
        }
    }
    else
    {
        switch (m_id)
        {
            case 0:
                PORTA &= ~(_BV(7));
                break;

            case 1:
                PORTC &= ~(_BV(3));
                break;

            case 2:
                PORTA &= ~(_BV(6));
                break;

            case 3:
                PORTC &= ~(_BV(1));
                break;

            default:
                assert(false);
        }
    }
}

bool CLed::Get()
{
    switch (m_id)
    {
        case 0:
            return (PORTA & _BV(7)) != 0;

        case 1:
            return (PORTC & _BV(3)) != 0;

        case 2:
            return (PORTA & _BV(6)) != 0;

        case 3:
            return (PORTC & _BV(1)) != 0;

        default:
            assert(false);
    }
}
