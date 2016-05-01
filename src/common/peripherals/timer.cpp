#include "common/peripherals/timer.h"

namespace OpenMAOR
{
namespace Peripherals
{

volatile uint8_t CTimer::m_timer0Counter;
volatile uint8_t CTimer::m_timer0InitValue;

ISR(TIMER0_OVF_vect)
{
    CTimer::m_timer0Counter++;
    TCNT0 = CTimer::m_timer0InitValue;
}

} // namespace Peripherals
} // namespace OpenMAOR
