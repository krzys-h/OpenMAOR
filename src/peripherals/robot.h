#pragma once

#include "peripherals/button.h"
#include "peripherals/led.h"
#include <avr/io.h>

class CFramework;

/**
 * \class CRobot
 * \brief Main CRobot instance, provides access to all peripherals
 */
class CRobot
{
public:
    CRobot()
    : led{
        CLed(&DDRA, &PORTA, 7),
        CLed(&DDRC, &PORTC, 3),
        CLed(&DDRA, &PORTA, 6),
        CLed(&DDRC, &PORTC, 1)
    }
    {}

public:
    CButton button;
    CLed led[4];
};
