#include "bootloader/protocol_avr109.h"
#include "common/uart.h"

#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

namespace OpenMAOR
{
namespace Protocols
{

uint8_t CProtocolAVR109Bootloader::GetParameterCount(AVR109Command command)
{
    if (command == START_MEMORY_WRITE && m_buffer.Size() >= 4)
    {
        return 3+((m_buffer[1] << 8) | m_buffer[2]);
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

bool CProtocolAVR109Bootloader::RecieveData(uint8_t data)
{
    m_buffer.Add(data);
    if (m_buffer.Size() >= GetParameterCount(static_cast<AVR109Command>(m_buffer[0]))+1)
    {
        ProcessPacket(static_cast<AVR109Command>(m_buffer[0]));
        m_buffer.Flush();
    }
    else
    {
        return true;
    }
    return false;
}

void CProtocolAVR109Bootloader::ProcessPacket(AVR109Command command)
{
    switch (command)
    {
        case PROGRAMMER_NAME:
            m_uart->SendString("MAOR12T");
            break;

        case PROGRAMMER_VERSION:
            m_uart->SendString("01"); // => 0.1
            break;

        case CHIP_SIGNATURE:
            m_uart->Send(SIGNATURE_2);
            m_uart->Send(SIGNATURE_1);
            m_uart->Send(SIGNATURE_0);
            break;

        case PROGRAMMER_TYPE:
            m_uart->Send('S'); // serial
            break;

        case PROGRAMMING_ENTER:
        case PROGRAMMING_LEAVE:
            m_uart->Send(OK);
            break;

        case PROGRAMMER_SUPPORTED_DEVICE_LIST:
            //m_uart->Send(0x75); // ATMega16, bootloader
            m_uart->Send(0x73); // ATMega32, bootloader
            m_uart->Send(0); // end of data
            break;

        case UNKNOWN1:
        case UNKNOWN2:
        case SET_DEVICE:
            m_uart->Send(OK);
            break;

        case PROGRAMMER_CHECK_BUFFER_SUPPORT:
            m_uart->Send('Y');
            m_uart->Send((SPM_PAGESIZE >> 8) & 0xFF);
            m_uart->Send(SPM_PAGESIZE & 0xFF);
            break;

        case SET_ADDRESS:
            m_address = (m_buffer[1] << 8) | m_buffer[2];
            m_uart->Send(OK);
            break;

        case PROGRAMMER_CHECK_AUTOINCREMENT:
            m_uart->Send('Y');
            break;

        case START_MEMORY_READ:
        case START_MEMORY_WRITE:
            StartMemoryOperation(command, static_cast<AVR109MemoryType>(m_buffer[3]), (m_buffer[1] << 8) | m_buffer[2]);
            break;

        case CHIP_ERASE:
            // Na razie to ignorujemy
            m_uart->Send(OK);
            break;

        case EXIT_BOOTLOADER:
            // Ignorujemy
            m_uart->Send(OK);
            break;

        default:
            m_uart->Send('?');
    }
}

void CProtocolAVR109Bootloader::StartMemoryOperation(AVR109Command command, AVR109MemoryType memory, uint16_t size)
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

void CProtocolAVR109Bootloader::StartMemoryRead(AVR109MemoryType memory, uint16_t size)
{
    if(memory == MEMORY_FLASH)
    {
        for (uint8_t i = 0; i < size; i += 2, m_address++)
        {
            uint16_t data = pgm_read_word(m_address << 1);
            m_uart->Send(data);
            m_uart->Send(data >> 8);
        }
    }
    else if(memory == MEMORY_EEPROM)
    {
        for (uint8_t i = 0; i < size; i++, m_address++)
        {
            m_uart->Send(eeprom_read_byte((uint8_t*)m_address));
        }
    }
}

void CProtocolAVR109Bootloader::StartMemoryWrite(AVR109MemoryType memory, uint16_t size)
{
    if (memory == MEMORY_FLASH)
    {
        for (uint8_t i = size; i < SPM_PAGESIZE; i++)
            m_buffer.Add(0xFF);

        uint16_t startAddress = m_address;
        boot_page_erase_safe(m_address << 1);
        for (uint16_t i = 0; i < SPM_PAGESIZE; i += 2, m_address++)
        {
            uint16_t data = (m_buffer[4+i] & 0xFF) | (m_buffer[4+i+1] << 8);
            boot_page_fill_safe(m_address << 1, data);
        }
        boot_page_write_safe(startAddress << 1);
        boot_rww_enable_safe();
        m_address = startAddress + (size/2);
        boot_spm_busy_wait();
    }
    else if (memory == MEMORY_EEPROM)
    {
        for (uint8_t i = 0; i < size; i++, m_address++)
        {
            eeprom_update_byte((uint8_t*)m_address, m_buffer[4+i]);
        }
    }
    m_uart->Send(OK);
}

} // namespace Protocols
} // namespace OpenMAOR
