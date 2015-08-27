#pragma once

#include "common/singleton.h"
#include "common/uart.h"
#include "common/protocols.h"
#include "common/remote_command.h"
#include "common/protocol_sparta.h"
#include "common/protocol_avr109.h"
#include "peripherals/peripherals.h"

/**
 * \class CFrameworkBase
 * \brief Provides base functionality of the framework, used in bootloader
 */
class CFrameworkBase : public CPeripherals, public CProtocols, protected CSingleton<CFrameworkBase>
{
friend class CSingleton<CFrameworkBase>;
public:
    CFrameworkBase();

    //! Sleeps until some interrupt occurs, this highly decreases power usage
    static void IdleSleep();
    //! In interrupt driven programs, call this when everything is configured and you want to halt the main() function
    static inline void InterruptDriven()
    {
        while (true)
        {
            IdleSleep();
        }
    }

    //! Power off the microcontroller. For use only in extreme cases (e.g. an unrecoverable error)
    static void PowerOff();

    //! Sleeps for given amount of milliseconds
    static void Sleep(uint16_t delay);

protected:
    static void UartCallback(uint8_t data);

public:
    CAsyncUart uart;

protected:
    CRemoteCommandExecutor m_command;

    CProtocolSparta m_protocolSparta;
};

/**
 * \class CRobot
 * \brief Main robot class. You should inherit from this class and implement your own program in it
 */
class CRobot : public CFrameworkBase
{
public:
    CRobot();

    static void ExitToBootloader();

private:
    CProtocolAVR109 m_protocolAVR109;
};
