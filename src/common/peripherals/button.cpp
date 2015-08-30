#include "common/peripherals/button.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

namespace OpenMAOR
{
namespace Peripherals
{

template<> CButton* CSingleton<CButton>::m_instance = nullptr;

CButton::CButton()
{
    DDRD &= ~(_BV(3));
    PORTD |= _BV(3);

    // Enable INT1
    GICR |= _BV(INT1);

    // Trigger INT1 on falling edge
    MCUCR |= _BV(ISC11);
    MCUCR &= ~(_BV(ISC10));
}

bool CButton::Get()
{
    return (PIND & _BV(3)) == 0;
}

CButton::ButtonCallback CButton::m_callback = nullptr;
void CButton::SetCallback(ButtonCallback callback)
{
    m_callback = callback;
}

CLASS_ISR(CButton, INT1_vect)
{
    _delay_ms(20); // drgania stykow sa glupie
    while (Get()); // czekamy na puszczenie
    _delay_ms(20);
    if (m_callback != nullptr)
    {
        m_callback();
    }
}

} // namespace Peripherals
} // namespace OpenMAOR
