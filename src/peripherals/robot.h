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
    CButton button;
    CLeds led;
};
