#include <avr/io.h>

#include "peripherals/leds.h"

#define LED0_CONFIG DDRA |= _BV(7)
#define LED0_OFF PORTA &= ~(_BV(7))
#define LED0_ON PORTA |= _BV(7)
#define LED0_GET ((PORTA & _BV(7)) != 0)

#define LED1_CONFIG DDRC |= _BV(3)
#define LED1_OFF PORTC &= ~(_BV(3))
#define LED1_ON PORTC |= _BV(3)
#define LED1_GET ((PORTC & _BV(3)) != 0)

#define LED2_CONFIG DDRA |= _BV(6)
#define LED2_OFF PORTA &= ~(_BV(6))
#define LED2_ON PORTA |= _BV(6)
#define LED2_GET ((PORTA & _BV(6)) != 0)

#define LED3_CONFIG DDRC |= _BV(1)
#define LED3_OFF PORTC &= ~(_BV(1))
#define LED3_ON PORTC |= _BV(1)
#define LED3_GET ((PORTC & _BV(1)) != 0)

void leds_init()
{
    LED0_CONFIG;
    LED1_CONFIG;
    LED2_CONFIG;
    LED3_CONFIG;
}

void set_led(unsigned int led, bool value)
{
    if(get_led(led) == value) return;
    if(value) {
        switch(led) {
            case 0:
                LED0_ON;
                break;
            case 1:
                LED1_ON;
                break;
            case 2:
                LED2_ON;
                break;
            case 3:
                LED3_ON;
                break;
        }
    } else {
        switch(led) {
            case 0:
                LED0_OFF;
                break;
            case 1:
                LED1_OFF;
                break;
            case 2:
                LED2_OFF;
                break;
            case 3:
                LED3_OFF;
                break;
        }
    }
}

bool get_led(unsigned int led)
{
    switch(led) {
        case 0:
            return LED0_GET;
        case 1:
            return LED1_GET;
        case 2:
            return LED2_GET;
        case 3:
            return LED3_GET;
    }
    return false;
}