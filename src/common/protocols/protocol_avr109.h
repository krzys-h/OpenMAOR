#pragma once

#include "common/protocols/protocol_base.h"

namespace OpenMAOR
{
namespace Protocols
{

/**
 * \class CProtocolAVR109
 * \brief Program-side implementation of AVR109 protocol
 *
 * The only task of this implementation is to jump to bootloader when AVR109 transmission starts
 */
class CProtocolAVR109 : public CBufferedProtocol
{
public:
    CProtocolAVR109(CUart* uart) : CBufferedProtocol(uart) {};

    bool RecieveData(uint8_t byte);
};

} // namespace Protocols
} // namespace OpenMAOR
