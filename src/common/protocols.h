#pragma once

#include "common/protocol_base.h"

class CProtocol;

/**
 * \class CProtocols
 * \brief Handles multiple communication protocols
 */
class CProtocols
{
public:
    void AddProtocol(CProtocol* protocol);
    bool RecieveData(uint8_t byte);

protected:
    static const int MAX_PROTOCOLS = 2;
    CBuffer<CProtocol*, MAX_PROTOCOLS> m_protocols;
    CProtocol* m_currentProtocol = nullptr;
};
