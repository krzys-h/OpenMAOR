#pragma once

#include "common/singleton.h"
#include "common/uart.h"

#include "common/peripherals/peripherals.h"

#include "common/protocols/protocols.h"
#include "common/protocols/protocol_sparta.h"
#include "common/protocols/protocol_avr109.h"

/**
 * \class CFrameworkBase
 * \brief Provides base functionality of the framework, used in bootloader
 */
class CFrameworkBase : public CPeripherals, public CProtocols, protected CSingleton<CFrameworkBase>
{
friend class CSingleton<CFrameworkBase>;
public:
    CFrameworkBase();
    CFrameworkBase(const CFrameworkBase&) = delete;
    CFrameworkBase& operator=(const CFrameworkBase&) = delete;

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

protected:
    static void UartCallback(uint8_t data);

public:
    CAsyncUart uart;

protected:
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
