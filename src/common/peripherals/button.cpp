#include "common/peripherals/button.h"

#include <avr/io.h>
#include <avr/interrupt.h>

template<> CButton* CSingleton<CButton>::m_instance = nullptr;

CButton::CButton()
{
    DDRD |= _BV(3);
    PORTD |= _BV(3);

    // Enable INT1
    GICR |= _BV(INT1);

    // Trigger INT1 on falling edge
    // TODO: Na pewno MCUCR? Czy moze GICR, jak z przekierowaniem IVT?
    MCUCR |= _BV(ISC11);
    MCUCR &= ~(_BV(ISC10));
}

bool CButton::Get()
{
    return (PIND & _BV(3)) == 0;
}

void CButton::SetCallback(ButtonCallback callback)
{
    m_callback = callback;
}

CLASS_ISR(CButton, INT1_vect)
{
    if (m_callback != nullptr)
    {
        m_callback();
    }
}
