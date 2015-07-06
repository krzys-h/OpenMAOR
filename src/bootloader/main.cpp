#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "bootloader/main.h"
#include "common/uart.h"
#include "bootloader/flash_prog.h"
#include "bootloader/protocol_avr109.h"
#include "common/protocol_sparta.h"

const static void (*program_main)(void) __attribute__((noreturn)) = 0;

volatile bool exit_bootloader = false;
int main()
{
    uart_init(false);

    DDRA |= _BV(0);
    PORTA &= ~(_BV(0));

    while(!exit_bootloader) {
        uint8_t cmd = uart_read();
        if(cmd == SPARTA_HEADER || cmd == SPARTA_HEADER_STATUS) {
            sparta_byte_recieved(cmd);
        } else {
            avr109_byte_recieved(cmd);
        }
    }
    eeprom_busy_wait();
    programming_busy_wait();
    while(!(UCSRA & (1<<UDRE))); // Czekamy na koniec transmisji UART

    program_main();
}
