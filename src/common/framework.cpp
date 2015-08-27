#include "common/framework.h"

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

template<> CFrameworkBase* CSingleton<CFrameworkBase>::m_instance = nullptr;

CFrameworkBase::CFrameworkBase()
    : uart(UartCallback)
    , m_command(this)
    , m_protocolSparta(&uart, &m_command)
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

void CFrameworkBase::PowerOff()
{
    cli();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    while (true)
    {
        sleep_cpu();
    }
}

void CFrameworkBase::Sleep(uint16_t delay)
{
    // TODO: Trzeba to zrobić na przerwaniach
    for (uint16_t i = 0; i < delay; i+=100)
        _delay_ms(100);
}

// =============================================================================

CRobot::CRobot()
    : CFrameworkBase()
    , m_protocolAVR109(&uart, &m_command)
{
    AddProtocol(&m_protocolAVR109);
    button.SetCallback(ExitToBootloader);
    sei();
}

// const static void (*jump_bootloader)(void) __attribute__((noreturn)) = (const void(*)())BOOTLOADER_START;
void CRobot::ExitToBootloader()
{
    cli();
    wdt_enable(WDTO_15MS);
    while (true);
}
