#include <avr/io.h>
#include "framework/framework.h"

int main()
{
    framework_init();
    while(true) {
        set_led(0, true);
        delay_ms(500);
        set_led(0, false);
        delay_ms(500);
    }
}