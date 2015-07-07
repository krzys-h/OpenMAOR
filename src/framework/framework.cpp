#include <avr/io.h>
#include <avr/interrupt.h>
#include "framework/framework.h"
#include "framework/button_exit.h"

void framework_init()
{
    peripherals_init();
    button_exit_init();

    uart_init(true);
    protocols_init();
    
    sei(); // Włączamy przerwania
}