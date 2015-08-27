#include "common/peripherals/timer.h"
#include "common/framework.h"

template<> CTimer* CSingleton<CTimer>::m_instance = nullptr;

CTimer::CTimer()
{
    StopTimer0();
}

void CTimer::Sleep(uint16_t delay)
{
    m_timer0Counter = 0;
    StartTimer0(255-125);
    while (m_timer0Counter < (delay/100)*125/10)
    {
        CFrameworkBase::IdleSleep();
    }
    StopTimer0();
}

void CTimer::StartTimer0(uint8_t initValue)
{
    m_timer0InitValue = initValue;
    TCCR0 |= (1<<CS02) | (0<<CS01) | (1<<CS00); // preskaler 1024 => czestotliwosc 1/1024 * F_CPU
    TIMSK |= (1<<TOIE0); // włączenie przerwania od przepełnienia licznika
    TCNT0 = m_timer0InitValue; // ustawienie wartości początkowej
}

void CTimer::StopTimer0()
{
    TCCR0 &= ~( (1<<CS02) | (1<<CS01) | (1<<CS00) );
    TIMSK &= ~(1<<TOIE0);
    TCNT0 = 0;
}

CLASS_ISR(CTimer, TIMER0_OVF_vect)
{
    m_timer0Counter++;
    TCNT0 = m_timer0InitValue;
}
