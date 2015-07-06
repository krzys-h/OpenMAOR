#include <avr/io.h>
#include "framework/protocols.h"
#include "common/protocol_sparta.h"
#include "framework/jump_bootloader.h"

void process_uart_byte(uint8_t byte)
{
    if(byte == SPARTA_HEADER || byte == SPARTA_HEADER_STATUS) {
        sparta_byte_recieved(byte);
    } else if(byte == 0x1B) { // ESC
        jump_bootloader();
    }
}