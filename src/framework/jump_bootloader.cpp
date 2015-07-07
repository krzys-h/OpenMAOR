#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "framework/jump_bootloader.h"
#include "peripherals/peripherals_init.h"

void exit_program()
{
    cli();
    peripherals_stop();
    //jump_bootloader();
    wdt_enable(WDTO_15MS);
}