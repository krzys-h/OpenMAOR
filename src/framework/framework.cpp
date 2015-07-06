#include <avr/io.h>
#include <avr/interrupt.h>
#include "framework/framework.h"

void framework_init()
{
    peripherals_init();

    uart_init(true);
    protocols_init();
    
    sei(); // Włączamy przerwania
}