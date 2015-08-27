#pragma once

#include "common/peripherals/button.h"
#include "common/peripherals/led.h"
#include "common/peripherals/timer.h"
#include "common/peripherals/motors.h"
#include "common/peripherals/line_sensor.h"

#include "common/extra_section.h"

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

    static CLineSensors lineSensor;

    static CTimer timer;
};
