#pragma once

#include "common/protocols/protocol_base.h"

class CProtocolAVR109 : public CBufferedProtocol
{
public:
    CProtocolAVR109(CUart* uart) : CBufferedProtocol(uart) {};

    bool RecieveData(uint8_t byte);
};
