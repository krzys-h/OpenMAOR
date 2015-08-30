#pragma once

#include "common/singleton.h"
#include "common/uart.h"

#include "common/peripherals/peripherals.h"

#include "common/protocols/protocols.h"
#include "common/protocols/protocol_sparta.h"

namespace OpenMAOR
{

/**
 * \class CFrameworkBase
 * \brief Provides base functionality of the framework, used also in bootloader
 */
class CFrameworkBase : public CPeripherals, public CProtocols, protected CSingleton<CFrameworkBase>
{
protected:
    friend class CSingleton<CFrameworkBase>;
    CFrameworkBase();

public:
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

private:
    static void UartCallback(uint8_t data);

public:
    CAsyncUart uart;

private:
    Protocols::CProtocolSparta m_protocolSparta;
};

} // namespace OpenMAOR
