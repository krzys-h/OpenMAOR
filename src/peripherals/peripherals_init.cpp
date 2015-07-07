#include "peripherals/peripherals_init.h"
#include "peripherals/leds.h"
#include "peripherals/button.h"

void peripherals_init()
{
    leds_init();
    button_init(true);

    peripherals_stop();
}

void peripherals_stop()
{
    set_led(0, false);
    set_led(1, false);
    set_led(2, false);
    set_led(3, false);
}