#pragma once

#include "common/stdlib/buffer.h"
#include <avr/boot.h> // dla SPM_PAGESIZE
#include <stdint.h>

namespace OpenMAOR
{
class CUart;

namespace Protocols
{

/**
 * \class CProtocol
 * \brief Protocol base class
 */
class CProtocol
{
public:
    CProtocol() {};
    CProtocol(const CProtocol&) = delete;
    CProtocol& operator=(const CProtocol&) = delete;

    //! Read the incoming byte. If this returns true, the next recieved byte will be immediately delivered here, skipping all other protocols.
    virtual bool RecieveData(uint8_t byte) = 0;
};

} // namespace Protocols
} // namespace OpenMAOR
