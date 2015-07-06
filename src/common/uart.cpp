#include <avr/io.h>
#include "common/uart.h"

#ifndef F_CPU
    #error Nie zdefiniowano F_CPU, potrzebne do obliczenia szybkosci transmisji
#endif
#define BAUD 57600

// http://hobby.abxyz.bplaced.net/index.php?pid=4&aid=8
void uart_init()
{
    // Ustalamy predkosc transmisji
    unsigned int ubbr = (F_CPU)/(BAUD*16UL)-1;
    UBRRH = (unsigned char)(ubbr>>8);
    UBRRL = (unsigned char)ubbr;

    // Odpalamy nadajnik i odbiornik
    UCSRB = (1<<RXEN) | (1<<TXEN);

    // Format ramki: 8 bitów danych, 1 bit stopu, brak bitu parzystości
    // Teoretycznie taki powinien byc domyślnie, ale na wszelki wypadek ustawiamy
    UCSRC = (1<<URSEL) | (3<<UCSZ0);
}

uint8_t uart_read()
{
    while(!(UCSRA & (1<<RXC)));
    return UDR;
}

void uart_write(uint8_t c)
{
    while(!(UCSRA & (1<<UDRE)));
    UDR = c;
}