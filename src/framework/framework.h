#pragma once

#include "common/singleton.h"
#include "common/uart.h"
#include "common/protocols.h"
#include "common/remote_command.h"
#include "common/protocol_sparta.h"
#include "common/protocol_avr109.h"
#include "peripherals/robot.h"
#include <avr/interrupt.h>
#include <avr/wdt.h>

/**
 * \class CFramework
 * \brief Main framework class. You should always create one (and only one!) instance of this class in your program
 */
class CFramework : public CSingleton<CFramework>
{
public:
    CFramework()
    : m_uart(UartCallback)
    , m_command(&m_robot)
    , m_protocolSparta(&m_uart, &m_command)
    , m_protocolAVR109(&m_uart, &m_command)
    {
        m_protocols.AddProtocol(&m_protocolSparta);
        m_protocols.AddProtocol(&m_protocolAVR109);
        sei();
    }

    CRobot& GetRobot()
    {
        return m_robot;
    }

    CUart& GetUart()
    {
        return m_uart;
    }

    // const static void (*jump_bootloader)(void) __attribute__((noreturn)) = (const void(*)())BOOTLOADER_START;
    void ExitToBootloader()
    {
        cli();
        wdt_enable(WDTO_15MS);
        while (true);
    }

protected:
    static void UartCallback(uint8_t data)
    {
        CFramework::GetInstance().m_protocols.RecieveData(data);
    }

protected:
    CRobot m_robot;
    CAsyncUart m_uart;

    CRemoteCommandExecutor m_command;
    CProtocols m_protocols;

    CProtocolSparta m_protocolSparta;
    CProtocolAVR109 m_protocolAVR109;
};
