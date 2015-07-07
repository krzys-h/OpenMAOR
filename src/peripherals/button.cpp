#include <avr/io.h>
#include <avr/interrupt.h>
#include "peripherals/button.h"

#define BUTTON_CONFIG DDRD |= _BV(3); PORTD |= _BV(3)
#define BUTTON_GET ((PIND & _BV(3)) == 0)

void button_init(bool interrupts)
{
    BUTTON_CONFIG;

    if(interrupts) {
        // Enable INT1
        GICR |= _BV(INT1);

        // Trigger INT1 on falling edge
        MCUCR |= _BV(ISC11);
        MCUCR &= ~(_BV(ISC10));
    }
}

bool button_pressed()
{
    return BUTTON_GET;
}