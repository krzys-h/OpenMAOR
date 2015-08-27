#include "common/robot.h"
#include <avr/wdt.h>

CRobot::CRobot()
    : CFrameworkBase()
    , m_protocolAVR109(&uart)
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
