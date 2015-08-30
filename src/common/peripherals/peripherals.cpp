#include "common/peripherals/peripherals.h"

namespace OpenMAOR
{

Peripherals::CButton CPeripherals::button;
Peripherals::CLeds CPeripherals::led;

Peripherals::CMotors CPeripherals::motors;

Peripherals::CLineSensors CPeripherals::lineSensor;
Peripherals::CSonar CPeripherals::sonar;

Peripherals::CTimer CPeripherals::timer;
Peripherals::CADC CPeripherals::adc;

} // namespace OpenMAOR
