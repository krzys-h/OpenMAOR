#include <avr/io.h>
#include "common/uart.h"

namespace OpenMAOR
{

CUart::RecvCallbackFunc CUart::m_callback = nullptr;

CLASS_ISR(CUart, USART_RXC_vect)
{
    uint8_t byte = UDR;
    if (CUart::m_callback != nullptr)
    {
        CUart::m_callback(byte);
    }
}

} // namespace OpenMAOR
