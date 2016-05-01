#pragma once

#include "common/peripherals/button.h"
#include "common/peripherals/led.h"
#include "common/peripherals/timer.h"
#include "common/peripherals/motors.h"
#include "common/peripherals/line_sensor.h"
#include "common/peripherals/sonar.h"
#include "common/peripherals/adc.h"

namespace OpenMAOR
{

/**
 * \class CPeripherals
 * \brief Provides access to all peripherals
 */
class CPeripherals
{
public:
    static Peripherals::CButton button;
    static Peripherals::CLeds led;

    static Peripherals::CMotors motors;

    static Peripherals::CLineSensors lineSensor;
    static Peripherals::CSonar sonar;

    static Peripherals::CTimer timer;
    static Peripherals::CADC adc;
};

} // namespace OpenMAOR
