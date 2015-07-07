#include <avr/interrupt.h>
#include <util/delay.h>
#include "framework/button_exit.h"
#include "peripherals/peripherals_init.h"
#include "peripherals/button.h"
#include "framework/jump_bootloader.h"

void button_exit_init()
{
    // This function is here only to force compiler to link this .o file with ISR into application
}

// Exit on button press
ISR(INT1_vect)
{
    cli(); // Stop any interrupts
    
    peripherals_stop();
    _delay_ms(50);
    while(button_pressed());
    _delay_ms(50);
    exit_program();
}