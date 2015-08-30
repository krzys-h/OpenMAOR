#include "common/framework.h"

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

namespace OpenMAOR
{

template<> CFrameworkBase* CSingleton<CFrameworkBase>::m_instance = nullptr;

CFrameworkBase::CFrameworkBase()
    : uart(UartCallback)
    , m_protocolSparta(&uart)
{
    AddProtocol(&m_protocolSparta);
}

void CFrameworkBase::UartCallback(uint8_t data)
{
    GetInstance().RecieveData(data);
}

void CFrameworkBase::IdleSleep()
{
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sleep_cpu();
    sleep_disable();
}

} // namespace OpenMAOR
