#include "common/uart.h"
#include "common/protocol_sparta.h"
#include "bootloader/protocol_avr109.h"
#include "common/protocols.h"
#include "common/remote_command.h"
#include "peripherals/robot.h"

const static void (*program_main)(void) __attribute__((noreturn)) = 0;

void MoveInterrupts(bool in_bootloader)
{
    // Zostawiam tu ten komentarz na pamiątkę jak parę godzin się męczyłem że na ATMega32 to jest GICR a _nie_ MCUCR :D
    GICR = (1 << IVCE);
    GICR = (in_bootloader << IVSEL);
    sei();
}

// TODO: To be extended with program saving/loading
class CBootloaderRemoteCommandExecutor : public CRemoteCommandExecutor {};

volatile bool exit_bootloader = false;
int main()
{
    // Przenosimy IVT do sekcji bootloadera
    MoveInterrupts(true);

    CRobot robot;
    robot.led[0].Set(true);

    CQueuedUart uart;
    CBootloaderRemoteCommandExecutor commandExecutor;
    CProtocolSparta sparta(&uart, &commandExecutor);
    CProtocolAVR109Bootloader avr109(&uart, &commandExecutor);
    CProtocols protocols;
    protocols.AddProtocol(&sparta);
    protocols.AddProtocol(&avr109);

    uart.SendString("\e[2J\e[H"); // czyszczenie terminala (+ kursor w lewy górny róg)
    uart.SendString("Bootloader INIT!\r\n");

    while (true)
    {
        protocols.RecieveData(uart.Recv());
    }
}
