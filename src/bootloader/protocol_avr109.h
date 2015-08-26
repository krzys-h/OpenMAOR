#pragma once

#include "common/protocol_avr109.h"

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

class CProtocolAVR109Bootloader : public CProtocolAVR109
{
public:
    CProtocolAVR109Bootloader(CUart* uart, CRemoteCommandExecutor* command) : CProtocolAVR109(uart, command) {};

    bool RecieveData(uint8_t byte);

protected:
    int GetParameterCount(AVR109Command command);
    void ProcessPacket(AVR109Command command);

    void StartMemoryOperation(AVR109Command command, AVR109MemoryType memory, uint16_t size);
    void StartMemoryRead(AVR109MemoryType memory, uint16_t size);
    void StartMemoryWrite(AVR109MemoryType memory, uint16_t size);

protected:
    uint16_t m_address = 0;
};
