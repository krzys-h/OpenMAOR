#include "common/peripherals/line_sensor.h"
#include <avr/io.h>

// TODO: This is untested!

namespace OpenMAOR
{
namespace Peripherals
{

CLineSensor::CLineSensor(volatile uint8_t& ddr, volatile uint8_t& port, volatile uint8_t& pin, uint8_t bit)
    : m_pin(pin)
    , m_bit(bit)
{
    ddr &= ~(_BV(m_bit));
    port &= ~(_BV(m_bit));
}

bool CLineSensor::Get()
{
    return (m_pin & _BV(m_bit)) != 0;
}

} // namespace Peripherals
} // namespace OpenMAOR
