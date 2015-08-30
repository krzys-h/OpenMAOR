#include "common/protocols/protocol_avr109.h"

namespace OpenMAOR
{
namespace Protocols
{

bool CProtocolAVR109::RecieveData(uint8_t data)
{
    if (data == 0x1B) // ESC
    {
        //TODO: m_command->StopProgram();
        return true; // should never get here, but anyway...
    }
    return false;
}

} // namespace Protocols
} // namespace OpenMAOR
