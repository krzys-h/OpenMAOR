#include "common/peripherals/motors.h"

// TODO: Test this code

namespace OpenMAOR
{
namespace Peripherals
{

int16_t CMotors::m_speedLeft = 0;
int16_t CMotors::m_speedRight = 0;

CMotors::CMotors()
{
    //TODO: sprawdzić
    TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM10)|(1<<WGM11);
    // Tryb 10-bit Fast PWM inverting mode,
    // clk/64, co daje okres ok 4ms
    TCCR1B|=(1<<WGM12)|(1<<CS11)|(1<<CS10);
    //TCCR1B|=(1<<WGM12)|(1<<CS12);

    // Wyjscia PWM
    DDRD |= _BV(5); // PWMA
    DDRD |= _BV(4); // PWMB

    // lewy
    DDRC |= _BV(6);
    DDRC |= _BV(5);

    // prawy
    DDRC |= _BV(2);
    DDRC |= _BV(4);
}

void CMotors::SetLeft(int16_t value)
{
    if (value > MAX_SPEED) value = MAX_SPEED;
    if (value < -MAX_SPEED) value = -MAX_SPEED;

    if (value > 0)
    {
        // do przodu - A=1, B=0
        PORTC |= _BV(6);
        PORTC &= ~(_BV(5));
        StartPWM_B();
        SetPWM_B(value);
    }
    else if (value < 0)
    {
        // do tylu - A=0, B=1
        PORTC &= ~(_BV(6));
        PORTC |= _BV(5);
        StartPWM_B();
        SetPWM_B(-value);
    }
    else
    {
        // STOP - A=0, B=0
        PORTC &= ~(_BV(6));
        PORTC &= ~(_BV(5));
        StopPWM_B();
        SetPWM_B(0);
    }

    m_speedLeft = value;
}

void CMotors::SetRight(int16_t value)
{
    if (value > MAX_SPEED) value = MAX_SPEED;
    if (value < -MAX_SPEED) value = -MAX_SPEED;

    if (value > 0)
    {
        // do przodu - A=1, B=0
        PORTC |= _BV(2);
        PORTC &= ~(_BV(4));
        StartPWM_A();
        SetPWM_A(value);
    }
    else if (value < 0)
    {
        // do tylu - A=0, B=1
        PORTC &= ~(_BV(2));
        PORTC |= _BV(4);
        StartPWM_A();
        SetPWM_A(-value);
    }
    else
    {
        // STOP - A=0, B=0
        PORTC &= ~(_BV(2));
        PORTC &= ~(_BV(4));
        StopPWM_A();
        SetPWM_A(0);
    }

    m_speedRight = value;
}

} // namespace Peripherals
} // namespace OpenMAOR
