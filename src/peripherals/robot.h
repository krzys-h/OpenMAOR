#pragma once

#include "peripherals/button.h"
#include "peripherals/led.h"

class CFramework;

/**
 * \class CRobot
 * \brief Main CRobot instance, provides access to all peripherals
 */
class CRobot
{
public:
    CRobot()
    : led{ CLed(0), CLed(1), CLed(2), CLed(3) }
    {}

public:
    CButton button;
    CLed led[4];
};
