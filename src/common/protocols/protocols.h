#pragma once

#include "common/protocols/protocol_base.h"

namespace OpenMAOR
{

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

    void AddProtocol(Protocols::CProtocol* protocol);
    bool RecieveData(uint8_t byte);

private:
    static const uint8_t MAX_PROTOCOLS = 2;
    StdLib::CBuffer<Protocols::CProtocol*, MAX_PROTOCOLS> m_protocols;
    Protocols::CProtocol* m_currentProtocol = nullptr;
};

} // namespace OpenMAOR
