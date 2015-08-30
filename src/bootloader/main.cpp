#include "common/framework.h"
#include "bootloader/protocol_avr109.h"

namespace OpenMAOR
{
namespace Bootloader
{

class CBootloader : public CFrameworkBase
{
public:
    CBootloader()
    : m_protocolAVR109(&uart)
    {
        m_instance = this;

        AddProtocol(&m_protocolAVR109);

        // Przenosimy IVT do sekcji bootloadera
        MoveInterrupts(true);
        sei();
    }

    void Run()
    {
        led.Set(false);
        uart.SendString("\e[2J\e[H"); // czyszczenie terminala (+ kursor w lewy górny róg)
        for(uint8_t i = 0; i < 4*2; i++)
        {
            led.Toggle();
            timer.Sleep(100);
        }
        uart.SendString("Bootloader INIT!\r\n");
        button.SetCallback([]() {
            m_instance->StartProgram();
        });

        InterruptDriven();
    }

    void StartProgram()
    {
        // Resetujemy porty
        DDRA = 0xFF;
        DDRB = 0xFF;
        DDRC = 0xFF;
        DDRD = 0xFF;
        PORTA = 0x00;
        PORTB = 0x00;
        PORTC = 0x00;
        PORTD = 0x00;

        // Resetujemy przerwania
        cli();
        MoveInterrupts(false);

        // I... startujemy!
        EnterProgram();
    }

protected:
    constexpr static void (*EnterProgram)(void) __attribute__((noreturn)) = 0;

    void MoveInterrupts(bool in_bootloader)
    {
        // Kompilator cos psuje z limitem max 4 cykli procesora miedzy instrukcjami "out", wiec zrobimy to recznie w ASMie D:
        asm volatile (
            "in r16, %[_GICR]\n"
            "mov r17, r16\n"
            "sbr r16, %[_IVCE]\n"
            "cbr r17, %[_IVSEL]\n"
            "ori r17, %[value]\n"
            "out %[_GICR], r16\n"
            "out %[_GICR], r17"
            :
            : [_GICR]  "I" (_SFR_IO_ADDR(GICR))
            , [_IVCE]  "I" (1 << IVCE)
            , [_IVSEL] "I" (1 << IVSEL)
            , [value]  "I" (in_bootloader << IVSEL)
            : "r16", "r17"
        );
    }

protected:
    Protocols::CProtocolAVR109Bootloader m_protocolAVR109;
    static CBootloader* m_instance;
};
CBootloader* CBootloader::m_instance = nullptr;

} // namespace Bootloader
} // namespace OpenMAOR

int main()
{
    DDRA = 0xFF;
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;

    OpenMAOR::Bootloader::CBootloader bootloader;
    bootloader.Run();
}
