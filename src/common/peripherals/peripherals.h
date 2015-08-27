#pragma once

#include "common/peripherals/button.h"
#include "common/peripherals/led.h"

/**
 * \class CPeripherals
 * \brief Provides access to all peripherals
 */
class CPeripherals
{
public:
    CButton button;
    CLeds led;
};
