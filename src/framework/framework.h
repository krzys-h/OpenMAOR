#include "framework/protocols.h"
#include "framework/jump_bootloader.h"
#include "peripherals/peripherals_init.h"
#include "peripherals/leds.h"
#include "peripherals/button.h"
#include "common/uart.h"
#include <util/delay.h>
#define delay_ms _delay_ms

void framework_init();