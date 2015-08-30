#pragma once

#include "common/stdlib/buffer.h"
#include <avr/boot.h> // dla SPM_PAGESIZE
#include <stdint.h>

class CUart;
class CRemoteCommandExecutor;

/**
 * \class CProtocol
 * \brief Protocol base class
 */
class CProtocol
{
public:
    CProtocol(CUart* uart) : m_uart(uart) {};
    CProtocol(const CProtocol&) = delete;
    CProtocol& operator=(const CProtocol&) = delete;

    //! Read the incoming byte. If this returns true, the next recieved byte will be immediately delivered here, skipping all other protocols.
    virtual bool RecieveData(uint8_t byte) = 0;

protected:
    CUart* m_uart;
};

/**
 * \class CBufferedProtocol
 * \brief Base class for protocols that need a buffer for a packet before its fully recieved and can be processed
 */
class CBufferedProtocol : public CProtocol
{
public:
    CBufferedProtocol(CUart* uart) : CProtocol(uart) {};

public:
    //! Maximum size of one packet
    static const uint8_t MAX_PACKET_SIZE = 4 + SPM_PAGESIZE;

protected:
    //! Recieved packet buffer
    /** NOTE: Because you can recieve packets only from one protocol at a time, this buffer is shared across all protocols. Be careful! **/
    static CBuffer<uint8_t, MAX_PACKET_SIZE> m_buffer;
};
