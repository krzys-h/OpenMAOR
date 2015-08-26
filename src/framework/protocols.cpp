#include <avr/io.h>
#include <avr/interrupt.h>
#include "framework/protocols.h"
//#include "common/protocol_sparta.h"
#include "framework/jump_bootloader.h"

void protocols_init()
{
    // This function is here only to force compiler to link this .o file with ISR into application
}

void protocols_process_byte(uint8_t byte)
{
    /*if(byte == SPARTA_HEADER || byte == SPARTA_HEADER_STATUS) {
        sparta_byte_recieved(byte);
    } else if(byte == 0x1B) { // ESC
        jump_bootloader();
    }*/
}

/*ISR(USART_RXC_vect)
{
    protocols_process_byte(UDR);
}*/
