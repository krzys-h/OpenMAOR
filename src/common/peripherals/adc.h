#pragma once
#include "common/class_isr.h"
#include <avr/io.h>

namespace OpenMAOR
{
class CPeripherals;

namespace Peripherals
{

/**
 * \class CADC
 * \brief Controls the analog-to-digital converter of the microcontroller. In short: It allows for measuring voltages
 */
class CADC
{
private:
    friend class OpenMAOR::CPeripherals;
    CADC();

public:
    /**
     * \enum ADCChannel
     * \brief ADC channel, see CADC::Get
     */
    enum ADCChannel
    {
        ADC_CUSTOM0 = 0, //!< Custom ADC0 channel
        ADC_CUSTOM1 = 1, //!< Custom ADC1 channel
        ADC_CUSTOM2 = 2, //!< Custom ADC2 channel
        ADC_BATTERY = 3, //!< Battery channel
        ADC_MOTOR_LEFT = 4, //!< Left motor channel
        ADC_MOTOR_RIGHT = 5, //!< Right motor channel
        TOTAL_ADC_CHANNELS
    };

    //! Return a value from given ADC channel. Keep in mind this value is averaged already.
    static uint16_t Get(ADCChannel channel);

    DECLARE_CLASS_ISR(ADC_vect);

private:
    static uint16_t m_measurements[TOTAL_ADC_CHANNELS];

    static uint8_t m_currentChannel;
    static uint16_t m_measuredTotal;
    static uint8_t m_measuredCount;
};

const int ADC_MOTOR_MAX_VALUE = 200;

} // namespace Peripherals
} // namespace OpenMAOR
