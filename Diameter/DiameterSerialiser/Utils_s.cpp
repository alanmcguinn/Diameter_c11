#include "Utils_s.h"

#include <arpa/inet.h>

namespace Diameter {
namespace Serialiser {
namespace internal {

void setField(std::uint32_t& field, std::uint8_t version, std::uint32_t length) {
	std::uint32_t shiftedVersion = version;
	shiftedVersion <<= 24;
	shiftedVersion |= length;
	field = htonl(shiftedVersion);
}

std::uint8_t getUpperByte(std::uint32_t field) {
	std::uint32_t hostOrderVersionLength = ntohl(field);
	hostOrderVersionLength &= 0xFF000000;
	hostOrderVersionLength >>= 24;
	return hostOrderVersionLength;
}

std::uint32_t getLower24Bytes(std::uint32_t field) {
	std::uint32_t hostOrderVersionLength = ntohl(field);
	hostOrderVersionLength &= 0x00FFFFFF;
	return hostOrderVersionLength;
}

}
}
}

