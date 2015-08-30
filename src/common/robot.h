#pragma once

#include "common/framework.h"
#include "common/protocols/protocol_avr109.h"

namespace OpenMAOR
{

/**
 * \class CRobot
 * \brief Main robot class. You should inherit from this class and implement your own program in it
 */
class CRobot : public CFrameworkBase
{
public:
    CRobot();

    static void ExitToBootloader();

private:
    Protocols::CProtocolAVR109 m_protocolAVR109;
};

} // namespace OpenMAOR
