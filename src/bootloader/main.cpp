#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bootloader/main.h"
#include "common/uart.h"
#include "bootloader/flash_prog.h"
#include "bootloader/protocol_avr109.h"
#include "common/protocol_sparta.h"
#include "peripherals/button.h"
#include "peripherals/leds.h"

const static void (*program_main)(void) __attribute__((noreturn)) = 0;

volatile bool exit_bootloader = false;
int main()
{
    uart_init(false);
    button_init(false);
    leds_init();

    bool force_bootloader = button_pressed();
    if(force_bootloader) {
        _delay_ms(50);
        while(button_pressed());
        _delay_ms(50);
    }
    
    int led = 0;
    while(!button_pressed()) {
        if(UCSRA & (1<<RXC) || force_bootloader) {
            set_leds(true, false, false, true);
            while(!exit_bootloader) {
                uint8_t data = uart_read();
                if(data == SPARTA_HEADER || data == SPARTA_HEADER_STATUS) {
                    sparta_byte_recieved(data);
                } else {
                    avr109_byte_recieved(data);
                }
            }
            exit_bootloader = false;
            led = 0;
        }
        force_bootloader = false;

        switch(led) {
            case 0: set_leds(true, false, false, false); break;
            case 1: set_leds(false, true, false, false); break;
            case 2: set_leds(false, false, false, true); break;
            case 3: set_leds(false, false, true, false); break;
        }
        _delay_ms(100);
        led++;
        if(led >= 4) led = 0;
    }
    set_leds(false, false, false, false);
    _delay_ms(50);
    while(button_pressed());
    _delay_ms(50);

    eeprom_busy_wait();
    programming_busy_wait();

    program_main();
}
