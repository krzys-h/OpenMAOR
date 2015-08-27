#include "common/protocols/protocol_avr109.h"

#include "common/remote_command.h"

bool CProtocolAVR109::RecieveData(uint8_t data)
{
    if (data == 0x1B) // ESC
    {
        m_command->StopProgram();
        return true; // should never get here, but anyway...
    }
    return false;
}
