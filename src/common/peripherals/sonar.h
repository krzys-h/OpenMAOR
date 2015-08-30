#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "common/extra_section.h"

namespace OpenMAOR
{
class CPeripherals;

namespace Peripherals
{

/**
 * \class CSonar
 * \brief Communication with sonar controller via SPI
 */
class CSonar
{
private:
    friend class OpenMAOR::CPeripherals;
    CSonar() EXTRA;

public:
    struct SonarResult
    {
        uint8_t left;
        uint8_t right;
    };
    SonarResult Get() EXTRA;
};

} // namespace Peripherals
} // namespace OpenMAOR
