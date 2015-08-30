#include <avr/io.h>
#include "common/uart.h"

#ifndef F_CPU
    #error Nie zdefiniowano F_CPU, potrzebne do obliczenia szybkosci transmisji
#endif
#define BAUD 57600

namespace OpenMAOR
{

// =============================================================================

template<> CUart* CSingleton<CUart>::m_instance = nullptr;

CUart::CUart()
{
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

CUart::~CUart()
{
}

CLASS_ISR(CUart, USART_RXC_vect)
{
    ByteRecieved(UDR);
}

void CUart::Send(uint8_t byte)
{
    // TODO: Wysyłanie na przerwaniach
    while(!(UCSRA & (1<<UDRE)));
    UDR = byte;
}

//TODO: Wysyłanie z progmem
void CUart::SendString(const char* string)
{
    while(*string != '\0')
    {
        Send(*string++);
    }
}

// =============================================================================

CAsyncUart::CAsyncUart(RecvCallbackFunc recvCallback)
: CUart()
, m_callback(recvCallback)
{}

void CAsyncUart::ByteRecieved(uint8_t byte)
{
    m_callback(byte);
}

// =============================================================================

} // namespace OpenMAOR
