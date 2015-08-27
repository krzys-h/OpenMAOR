#include "common/peripherals/sonar.h"

// TODO: This code is untested!

// TODO: Separate file for SPI?

inline void SPIOn()
{
    // preskaler 64x
    //TODO: sprawdzić konfigurację
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1);
}

inline void SPIOff()
{
    SPCR = 0;
}

inline uint8_t SPIReadWrite(uint8_t data)
{
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}

inline uint8_t SPIRead()
{
    return SPIReadWrite(0x00);
}

// =============================================================================

CSonar::CSonar()
{
    DDRC |= _BV(7); // CS
    PORTC |= _BV(7);

    DDRB |= _BV(7); // SCK
    DDRB |= _BV(4); // SS
    DDRB |= _BV(5); // MOSI
    DDRB &= ~(_BV(6)); // MOSI
    PORTB &= ~(_BV(6));
    SPIOff();
}

CSonar::SonarResult CSonar::Get()
{
    SonarResult s;

    SPIOn();
    PORTC &= ~(_BV(7));
    _delay_us(5);
    s.left = SPIRead();
    _delay_us(5);
    s.right = SPIRead();
    PORTC |= _BV(7);
    SPIOff();

    return s;
}
