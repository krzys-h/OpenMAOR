#include "common/protocols/protocol_avr109.h"

#include "common/robot.h"

namespace OpenMAOR
{
namespace Protocols
{

bool CProtocolAVR109::RecieveData(uint8_t data)
{
    if (data == 0x1B) // ESC
    {
        CRobot::ExitToBootloader();
        return true; // should never get here, but anyway...
    }
    return false;
}

} // namespace Protocols
} // namespace OpenMAOR
