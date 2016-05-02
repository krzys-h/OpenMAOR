#include "common/peripherals/adc.h"

namespace OpenMAOR
{
namespace Peripherals
{

uint16_t CADC::m_measurements[TOTAL_ADC_CHANNELS];

uint8_t CADC::m_currentChannel = 0;
uint16_t CADC::m_measuredTotal = 0;
uint8_t CADC::m_measuredCount = 0;

/*
 * przy takiej ilosci pomiarow (przy zalozonym trybie pracy ADC)
 * otrzymujemy probki z jednego okresu
 * modulacji PWM. Dzieki temu mozna obliczyc wartosc srednia
 * pradu w okresie
 *
 * ~ dokumentacja kodu od producenta
 */
const int ADC_MEASUREMENT_COUNT = 38;

CLASS_ISR(CADC, ADC_vect)
{
    m_measuredTotal += ADC;
    if (++m_measuredCount == ADC_MEASUREMENT_COUNT)
    {
        ADCSRA &= ~(1<<ADEN); // teoretycznie zapobiega "mieszaniu sie" kanalow

        m_measurements[m_currentChannel] = m_measuredTotal / ADC_MEASUREMENT_COUNT;

        m_measuredTotal = 0;
        m_measuredCount = 0;

        m_currentChannel ++;
        if (m_currentChannel >= TOTAL_ADC_CHANNELS)
            m_currentChannel = 0;

        ADMUX = (1<<REFS0)|(1<<REFS1) | m_currentChannel;

        ADCSRA |= (1<<ADEN);
        ADCSRA |= (1<<ADSC);
    }
}

} // namespace Peripherals
} // namespace OpenMAOR
