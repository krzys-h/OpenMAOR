#include "common/protocols/protocols.h"

namespace OpenMAOR
{

void CProtocols::AddProtocol(Protocols::CProtocol* protocol)
{
    m_protocols.Add(protocol);
}

bool CProtocols::RecieveData(uint8_t byte)
{
    if (m_currentProtocol != nullptr)
    {
        if (!m_currentProtocol->RecieveData(byte))
        {
            m_currentProtocol = nullptr;
        }
        return true;
    }

    for (uint8_t i = 0; i < m_protocols.Size(); i++)
    {
        if (m_protocols[i]->RecieveData(byte))
        {
            m_currentProtocol = m_protocols[i];
            return true;
        }
    }
    return false;
}

} // namespace OpenMAOR
