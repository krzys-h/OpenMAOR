#pragma once

#include "common/singleton.h"
#include "common/class_isr.h"
#include "common/stdlib/queue.h"


#ifndef F_CPU
#error Nie zdefiniowano F_CPU, potrzebne do obliczenia szybkosci transmisji
#endif
#define BAUD 57600

namespace OpenMAOR
{

/**
 * \class CUart
 * \brief Class implementing UART communication (interrupt-driven)
 */
class CUart
{
public:
    typedef void(*RecvCallbackFunc)(uint8_t);

    CUart(RecvCallbackFunc recvCallback = nullptr)
    {
        m_callback = recvCallback;

        // Ustalamy predkosc transmisji
        uint16_t ubbr = (F_CPU)/(BAUD*16UL)-1;
        UBRRH = (uint8_t)(ubbr>>8);
        UBRRL = (uint8_t)ubbr;

        // Odpalamy nadajnik i odbiornik
        UCSRB = (1<<RXEN) | (1<<TXEN);

        // Włączamy przerwania odbiornika
        UCSRB |= (1<<RXCIE);

        // Format ramki: 8 bitów danych, 1 bit stopu, brak bitu parzystości
        // Teoretycznie taki powinien byc domyślnie, ale na wszelki wypadek ustawiamy
        UCSRC = (1<<URSEL) | (3<<UCSZ0);
    }

    CUart(const CUart&) = delete;
    CUart& operator=(const CUart&) = delete;

    DECLARE_CLASS_ISR(USART_RXC_vect);

    //! Waits for previous transmission to finish and then sends one byte via UART
    static void Send(uint8_t byte)
    {
        // TODO: Wysyłanie na przerwaniach
        while(!(UCSRA & (1<<UDRE)));
        UDR = byte;
    }

    //! Sends a null-terminated string
    static void SendString(const char* string)
    {
        while(*string != '\0')
        {
            Send(*string++);
        }
    }

    //TODO: Wysyłanie z progmem

private:
    static RecvCallbackFunc m_callback;
};

} // namespace OpenMAOR
