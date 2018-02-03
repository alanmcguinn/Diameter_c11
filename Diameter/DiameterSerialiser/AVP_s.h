/*
 * AVP_s.h
 *
 *  Created on: 7 Aug 2016
 *      Author: alanmc
 */

#ifndef AVP_S_H_
#define AVP_S_H_

#include <cstdint>
#include <cstring>
#include "Utils_s.h"

namespace Diameter {
namespace Serialiser {
namespace internal {

#define VBIT_MASK 0x80

struct AVP {
    std::uint32_t m_tag;
    std::uint32_t m_flagsLength;
    char m_value[0];

    void setFlagsLength(std::uint8_t flags, std::uint32_t length) {
        setField(m_flagsLength, flags, length);
    }

    std::uint32_t setVendorId(std::uint32_t vendorId) {
        if (vendorId > 0) {
            setFlagsLength(getFlags() | VBIT_MASK, getLength());
            uint32_t otwVendorId = htonl(vendorId);
            std::memcpy(m_value, &otwVendorId, sizeof(std::uint32_t));
            return sizeof(std::uint32_t);
        }
        return 0;
    }

    std::uint32_t getLength() const {
        return getLower24Bytes(m_flagsLength);
    }

    std::uint8_t getFlags() const {
        return getUpperByte(m_flagsLength);
    }

    std::uint32_t getCommandCode() {
        return ntohl(m_tag);
    }
};


}
}
}



#endif /* AVP_S_H_ */
