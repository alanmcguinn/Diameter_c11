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

struct AVP {
    std::uint32_t m_tag;
    union {
        struct {
            std::uint32_t m_vBit:1;
            std::uint32_t m_mBit:1;
            std::uint32_t m_pBit:1;
            std::uint32_t :5;
            std::uint32_t m_length:24;
        };
        std::uint32_t m_flagsLength;
    };
    char m_value[0];

//    void setFlagsLength(std::uint8_t flags, std::uint32_t length) {
//		setField(m_flagsLength, flags, length);
 //   }

    std::uint32_t setVendorId(std::uint32_t vendorId) {
        if (vendorId > 0) {
            m_vBit = 1;
            std::memcpy(m_value, &vendorId, sizeof(std::uint32_t));
            return sizeof(std::uint32_t);
        }
        return 0;
    }

    std::uint32_t getLength() {
		return m_length;
    }

    std::uint32_t getCommandCode() {
        return ntohl(m_tag);
    }
};


}
}
}



#endif /* AVP_S_H_ */
