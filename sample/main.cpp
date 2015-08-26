#include <avr/io.h>
#include "framework/framework.h"

CFramework framework;

int main()
{
    while(true) {
        set_led(0, true);
        delay_ms(500);
        set_led(0, false);
        delay_ms(500);
    }
}
