#include "common/protocols/protocol_base.h"

namespace OpenMAOR
{
namespace Protocols
{

StdLib::CBuffer<uint8_t, CBufferedProtocol::MAX_PACKET_SIZE> CBufferedProtocol::m_buffer;

} // namespace Protocols
} // namespace OpenMAOR
