#pragma once

#include "common/protocols/protocol_avr109.h"

namespace OpenMAOR
{
namespace Protocols
{

class CProtocolAVR109Bootloader : public CProtocolAVR109
{
public:
    bool RecieveData(uint8_t byte);
};

} // namespace Protocols
} // namespace OpenMAOR
