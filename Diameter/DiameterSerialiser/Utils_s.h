#ifndef UTILS_S_H
#define UTILS_S_H

#include <cstdint>

namespace Diameter {
namespace Serialiser {
namespace internal {

void setField(std::uint32_t& field, std::uint8_t version, std::uint32_t length);
std::uint8_t getUpperByte(std::uint32_t field);
std::uint32_t getLower24Bytes(std::uint32_t field);

}
}
}

#endif /* UTILS_S_H */
