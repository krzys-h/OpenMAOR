#include "common/peripherals/button.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

namespace OpenMAOR
{
namespace Peripherals
{

CButton::ButtonCallback CButton::m_callback = nullptr;

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
