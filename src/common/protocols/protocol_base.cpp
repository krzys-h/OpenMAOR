#include "common/protocols/protocol_base.h"

StdLib::CBuffer<uint8_t, CBufferedProtocol::MAX_PACKET_SIZE> CBufferedProtocol::m_buffer;
