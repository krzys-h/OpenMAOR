#pragma once

#include "common/protocols/protocol_base.h"

class CProtocol;

/**
 * \class CProtocols
 * \brief Handles multiple communication protocols
 */
class CProtocols
{
protected:
    CProtocols() {};

public:
    CProtocols(const CProtocols&) = delete;
    CProtocols& operator=(const CProtocols&) = delete;

    void AddProtocol(CProtocol* protocol);
    bool RecieveData(uint8_t byte);

protected:
    static const uint8_t MAX_PROTOCOLS = 2;
    CBuffer<CProtocol*, MAX_PROTOCOLS> m_protocols;
    CProtocol* m_currentProtocol = nullptr;
};
