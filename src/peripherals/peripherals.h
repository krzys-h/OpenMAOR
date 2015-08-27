#pragma once

#include "peripherals/button.h"
#include "peripherals/led.h"

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
