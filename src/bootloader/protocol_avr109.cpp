#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "platform.h"
#include "common/uart.h"
#include "bootloader/flash_prog.h"
#include "bootloader/main.h"

uint16_t avr109_address = 0;

void avr109_byte_recieved(uint8_t cmd)
{
    if(cmd == 'S') {
        uart_write(NAME_1);
        uart_write(NAME_2);
        uart_write(NAME_3);
        uart_write(NAME_4);
        uart_write(NAME_5);
        uart_write(NAME_6);
        uart_write(NAME_7);
    } else if(cmd == 'V') {
        uart_write('0'+PROG_VERSION_MAJOR);
        uart_write('0'+PROG_VERSION_MINOR);
    } else if(cmd == 's') {
        uart_write(SIGNATURE_2);
        uart_write(SIGNATURE_1);
        uart_write(SIGNATURE_0);
    } else if(cmd == 'p') { // programmer type
        uart_write('S'); // serial
    } else if(cmd == 'P') { // enter programming
        uart_write('\r');
    } else if(cmd == 'L') { // leave programming
        uart_write('\r');
    } else if(cmd == 't') {
        uart_write(PROG_TYPE);
        uart_write(0);
    } else if(cmd == 'x' || cmd == 'y' || cmd == 'T') {
        uart_read();
        uart_write('\r');
    } else if(cmd == 'b') {
        uart_write('Y');
        uart_write((SPM_PAGESIZE >> 8) & 0xFF);
        uart_write(SPM_PAGESIZE & 0xFF);
    } else if(cmd == 'A') {
        avr109_address = uart_read();
        avr109_address = (avr109_address << 8) | uart_read();
        uart_write('\r');
    } else if(cmd == 'a') {
        uart_write('Y');
    } else if(cmd == 'g') {
        uint16_t size = uart_read();
        size = (size << 8) | uart_read();
        uint8_t memtype = uart_read();
        if(memtype == 'F') {
            uint16_t data;
            do {
                data = pgm_read_word(avr109_address << 1);
                uart_write(data);
                uart_write(data >> 8);
                avr109_address++;
                size -= 2;
            } while(size > 0);
        } else if(memtype == 'E') {
            do {
                uart_write(eeprom_read_byte((uint8_t*)avr109_address));
                avr109_address++;
                size--;
            } while(size > 0);
        }
    } else if(cmd == 'B') {
        uint16_t size = uart_read();
        size = (size << 8) | uart_read();
        uint8_t memtype = uart_read();

        if(memtype == 'F') {
            uint8_t* tmp = page_buffer;
            for (uint8_t cnt = 0; cnt < SPM_PAGESIZE; cnt++) {
                *tmp++ = (cnt < size) ? uart_read() : 0xFF;
            }

            flash_program_page(avr109_address << 1);
            avr109_address += size/2;
        } else if(memtype == 'E') {
            do {
                eeprom_update_byte((uint8_t*)avr109_address, uart_read());
                avr109_address++;
                size--;
            } while(size > 0);
        }
        uart_write('\r');
    } else if(cmd == 'e') {
        //TODO: chip erase
        uart_write('\r');
    } else if(cmd == 'E') { // return to program
        exit_bootloader = true;
        uart_write('\r');
    } else {
        uart_write('?');
    }
}