#include "bootloader/protocol_avr109.h"
#include "common/uart.h"

#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

namespace OpenMAOR
{
namespace Protocols
{

namespace // anonymous namespace
{

enum AVR109Command
{
    PROGRAMMER_NAME = 'S',
    PROGRAMMER_VERSION = 'V',
    CHIP_SIGNATURE = 's',
    PROGRAMMER_TYPE = 'p',
    PROGRAMMING_ENTER = 'P',
    PROGRAMMING_LEAVE = 'L',
    PROGRAMMER_SUPPORTED_DEVICE_LIST = 't',
    SET_DEVICE = 'T',

    // TODO: Sprawdzić co to miało robić :P
    UNKNOWN1 = 'x',
    UNKNOWN2 = 'y',

    PROGRAMMER_CHECK_BUFFER_SUPPORT = 'b',
    SET_ADDRESS = 'A',
    PROGRAMMER_CHECK_AUTOINCREMENT = 'a',

    START_MEMORY_READ = 'g',
    START_MEMORY_WRITE = 'B',
    CHIP_ERASE = 'e',

    EXIT_BOOTLOADER = 'E',


    OK = '\r'
};

enum AVR109MemoryType
{
    MEMORY_FLASH = 'F',
    MEMORY_EEPROM = 'E'
};
    
//! Maximum size of one packet
const uint8_t MAX_PACKET_SIZE = 4 + SPM_PAGESIZE;

//! Recieved packet buffer
StdLib::CBuffer<uint8_t, MAX_PACKET_SIZE> buffer;

uint8_t GetParameterCount(AVR109Command command)
{
    if (command == START_MEMORY_WRITE && buffer.Size() >= 4)
    {
        return 3+((buffer[1] << 8) | buffer[2]);
    }

    switch (command)
    {
        case UNKNOWN1:
        case UNKNOWN2:
        case SET_DEVICE:
            return 1;

        case SET_ADDRESS:
            return 2;

        case START_MEMORY_READ:
        case START_MEMORY_WRITE:
            return 3;

        default:
            return 0;
    }
}

uint16_t address = 0;

void StartMemoryRead(AVR109MemoryType memory, uint16_t size)
{
    if(memory == MEMORY_FLASH)
    {
        for (uint8_t i = 0; i < size; i += 2, address++)
        {
            uint16_t data = pgm_read_word(address << 1);
            CUart::Send(data);
            CUart::Send(data >> 8);
        }
    }
    else if(memory == MEMORY_EEPROM)
    {
        for (uint8_t i = 0; i < size; i++, address++)
        {
            CUart::Send(eeprom_read_byte((uint8_t*)address));
        }
    }
}

void StartMemoryWrite(AVR109MemoryType memory, uint16_t size)
{
    if (memory == MEMORY_FLASH)
    {
        for (uint8_t i = size; i < SPM_PAGESIZE; i++)
            buffer.Add(0xFF);

        uint16_t startAddress = address;
        boot_page_erase_safe(address << 1);
        for (uint16_t i = 0; i < SPM_PAGESIZE; i += 2, address++)
        {
            uint16_t data = (buffer[4+i] & 0xFF) | (buffer[4+i+1] << 8);
            boot_page_fill_safe(address << 1, data);
        }
        boot_page_write_safe(startAddress << 1);
        boot_rww_enable_safe();
        address = startAddress + (size/2);
        boot_spm_busy_wait();
    }
    else if (memory == MEMORY_EEPROM)
    {
        for (uint8_t i = 0; i < size; i++, address++)
        {
            eeprom_update_byte((uint8_t*)address, buffer[4+i]);
        }
    }
    CUart::Send(OK);
}

void StartMemoryOperation(AVR109Command command, AVR109MemoryType memory, uint16_t size)
{
    if (command == START_MEMORY_READ)
    {
        StartMemoryRead(memory, size);
    }
    else if (command == START_MEMORY_WRITE)
    {
        StartMemoryWrite(memory, size);
    }
}

void ProcessPacket(AVR109Command command)
{
    switch (command)
    {
        case PROGRAMMER_NAME:
            CUart::SendString("MAOR12T");
            break;

        case PROGRAMMER_VERSION:
            CUart::SendString("01"); // => 0.1
            break;

        case CHIP_SIGNATURE:
            CUart::Send(SIGNATURE_2);
            CUart::Send(SIGNATURE_1);
            CUart::Send(SIGNATURE_0);
            break;

        case PROGRAMMER_TYPE:
            CUart::Send('S'); // serial
            break;

        case PROGRAMMING_ENTER:
        case PROGRAMMING_LEAVE:
            CUart::Send(OK);
            break;

        case PROGRAMMER_SUPPORTED_DEVICE_LIST:
            //CUart::Send(0x75); // ATMega16, bootloader
            CUart::Send(0x73); // ATMega32, bootloader
            CUart::Send(0); // end of data
            break;

        case UNKNOWN1:
        case UNKNOWN2:
        case SET_DEVICE:
            CUart::Send(OK);
            break;

        case PROGRAMMER_CHECK_BUFFER_SUPPORT:
            CUart::Send('Y');
            CUart::Send((SPM_PAGESIZE >> 8) & 0xFF);
            CUart::Send(SPM_PAGESIZE & 0xFF);
            break;

        case SET_ADDRESS:
            address = (buffer[1] << 8) | buffer[2];
            CUart::Send(OK);
            break;

        case PROGRAMMER_CHECK_AUTOINCREMENT:
            CUart::Send('Y');
            break;

        case START_MEMORY_READ:
        case START_MEMORY_WRITE:
            StartMemoryOperation(command, static_cast<AVR109MemoryType>(buffer[3]), (buffer[1] << 8) | buffer[2]);
            break;

        case CHIP_ERASE:
            // Na razie to ignorujemy
            CUart::Send(OK);
            break;

        case EXIT_BOOTLOADER:
            // Ignorujemy
            CUart::Send(OK);
            break;

        default:
            CUart::Send('?');
    }
}

}

bool CProtocolAVR109Bootloader::RecieveData(uint8_t data)
{
    buffer.Add(data);
    if (buffer.Size() >= GetParameterCount(static_cast<AVR109Command>(buffer[0]))+1)
    {
        ProcessPacket(static_cast<AVR109Command>(buffer[0]));
        buffer.Flush();
    }
    else
    {
        return true;
    }
    return false;
}

} // namespace Protocols
} // namespace OpenMAOR
