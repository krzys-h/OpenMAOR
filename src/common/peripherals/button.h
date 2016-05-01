#pragma once
#include "common/class_isr.h"


namespace OpenMAOR
{
class CPeripherals;

namespace Peripherals
{

/**
 * \class CButton
 * \brief A button! :D
 */
class CButton
{
private:
    friend class OpenMAOR::CPeripherals;
    CButton()
    {
        DDRD &= ~(_BV(3));
        PORTD |= _BV(3);

        // Enable INT1
        GICR |= _BV(INT1);

        // Trigger INT1 on falling edge
        MCUCR |= _BV(ISC11);
        MCUCR &= ~(_BV(ISC10));
    }

public:
    static bool Get()
    {
        return (PIND & _BV(3)) == 0;
    }

    typedef void(*ButtonCallback)();
    static void SetCallback(ButtonCallback callback)
    {
        m_callback = callback;
    }

    DECLARE_CLASS_ISR(INT1_vect);

private:
    static ButtonCallback m_callback;
};

} // namespace Peripherals
} // namespace OpenMAOR
