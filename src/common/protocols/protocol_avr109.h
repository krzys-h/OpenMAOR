#pragma once

#include "common/protocols/protocol_base.h"

class CProtocolAVR109 : public CBufferedProtocol
{
public:
    CProtocolAVR109(CUart* uart, CRemoteCommandExecutor* command) : CBufferedProtocol(uart, command) {};

    bool RecieveData(uint8_t byte);
};
