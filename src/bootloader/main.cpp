#include "common/framework.h"
#include "bootloader/protocol_avr109.h"

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
            "ori r16, %[value1]\n"
            "ori r17, %[value2]\n"
            "out %[_GICR], r16\n"
            "out %[_GICR], r17"
            :
            : [_GICR]  "I" (_SFR_IO_ADDR(GICR))
            , [value1] "I" (1 << IVCE)
            , [value2] "I" (in_bootloader << IVSEL)
            : "r16", "r17"
        );
    }

protected:
    CProtocolAVR109Bootloader m_protocolAVR109;
    static CBootloader* m_instance;
};
CBootloader* CBootloader::m_instance = nullptr;

int main()
{
    DDRA = 0xFF;
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;

    CBootloader bootloader;
    bootloader.Run();
}
