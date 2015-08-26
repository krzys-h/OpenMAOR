#include "common/uart.h"
#include "common/protocol_sparta.h"
#include "bootloader/protocol_avr109.h"
#include "common/protocols.h"
#include "common/remote_command.h"
#include "peripherals/robot.h"
#include <avr/sleep.h>

const static void (*program_main)(void) __attribute__((noreturn)) = 0;

void MoveInterrupts(bool in_bootloader)
{
    // Zostawiam tu ten komentarz na pamiątkę jak parę godzin się męczyłem że na ATMega32 to jest GICR a _nie_ MCUCR :D
    GICR = (1 << IVCE);
    GICR = (in_bootloader << IVSEL);
    sei();
}

CProtocols protocols;
void RecieveHandler(uint8_t data)
{
    protocols.RecieveData(data);
}

int main()
{
    // Przenosimy IVT do sekcji bootloadera
    MoveInterrupts(true);

    CRobot robot;
    robot.led.Set(false);

    CAsyncUart uart(RecieveHandler);
    CRemoteCommandExecutor commandExecutor(&robot);
    CProtocolSparta sparta(&uart, &commandExecutor);
    CProtocolAVR109Bootloader avr109(&uart, &commandExecutor);
    protocols.AddProtocol(&sparta);
    protocols.AddProtocol(&avr109);

    uart.SendString("\e[2J\e[H"); // czyszczenie terminala (+ kursor w lewy górny róg)
    uart.SendString("Bootloader INIT!\r\n");

    while (true)
    {
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_enable();
        sleep_cpu();
        sleep_disable();
    }
}
