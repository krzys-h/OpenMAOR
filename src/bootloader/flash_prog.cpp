#include <avr/io.h>
#include <avr/boot.h>
#include "bootloader/flash_prog.h"

uint8_t page_buffer[SPM_PAGESIZE];

void flash_program_page(uint16_t address)
{
    uint8_t* data_ptr = page_buffer;
    uint16_t data;
    boot_page_erase_safe(address);
    uint8_t size = SPM_PAGESIZE;
    uint16_t current_address = address;
    do {
        data = *data_ptr++;
        data |= *data_ptr++ << 8;
        boot_page_fill_safe(current_address, data);
        current_address += 2;
        size -= 2;
    } while(size > 0);
    boot_page_write_safe(address);
    boot_rww_enable_safe();
}

void programming_busy_wait()
{
    boot_spm_busy_wait();
}