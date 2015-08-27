#include "common/framework.h"
#include "bootloader/protocol_avr109.h"

class CBootloader : public CFrameworkBase
{
public:
    CBootloader()
    : m_protocolAVR109(&uart, &m_command)
    {
        AddProtocol(&m_protocolAVR109);

        // Przenosimy IVT do sekcji bootloadera
        MoveInterrupts(true);
        sei();
    }

    void Run()
    {
        led.Set(false);
        uart.SendString("\e[2J\e[H"); // czyszczenie terminala (+ kursor w lewy górny róg)
        uart.SendString("Bootloader INIT!\r\n");

        InterruptDriven();
    }

    constexpr static void (*EnterProgram)(void) __attribute__((noreturn)) = 0;

protected:
    void MoveInterrupts(bool in_bootloader)
    {
        // Zostawiam tu ten komentarz na pamiątkę jak parę godzin się męczyłem że na ATMega32 to jest w GICR a _nie_ MCUCR :D
        GICR = (1 << IVCE);
        GICR = (in_bootloader << IVSEL);
    }

protected:
    CProtocolAVR109Bootloader m_protocolAVR109;
};

int main()
{
    CBootloader bootloader;
    bootloader.Run();
}
