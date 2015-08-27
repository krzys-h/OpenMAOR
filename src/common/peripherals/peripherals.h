#pragma once

#include "common/peripherals/button.h"
#include "common/peripherals/led.h"
#include "common/peripherals/timer.h"
#include "common/peripherals/motors.h"

/**
 * \class CPeripherals
 * \brief Provides access to all peripherals
 */
class CPeripherals
{
public:
    static CButton button;
    static CLeds led;

    static CMotors motors;

    static CTimer timer;
};
