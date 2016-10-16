/*
 * AVP.h
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#ifndef AVP_H_
#define AVP_H_

#include <cstdint>
#include <memory>

namespace Diameter {
namespace Dictionary {

enum class Mandatory { must, mustnot, may, maynot, shouldnot };
enum class Protected { must, mustnot, may, maynot };

class AVPRaw {
public:
    AVPRaw(const std::string& name, std::uint32_t avpCode, std::uint32_t vendorId, Mandatory mbit, Protected pbit) :
            m_name(name), m_avpCode(avpCode), m_vendorId(vendorId), m_mbit(mbit), m_pbit(pbit) {
    }

    const std::string& getName() const {
    	return m_name;
    }

    std::uint32_t getAvpCode() const {
        return m_avpCode;
    }

    std::uint32_t getVendorId() const {
        return m_vendorId;
    }

    Mandatory getMbit() const {
        return m_mbit;
    }

    Protected getPbit() const {
        return m_pbit;
    }

private:
    std::string m_name;
    std::uint32_t m_avpCode;
    std::uint32_t m_vendorId;
    Mandatory m_mbit;
    Protected m_pbit;
};

using AVP = std::shared_ptr<AVPRaw>;

AVP makeAVP(const std::string& name, std::uint32_t avpCode, std::uint32_t vendorId, Mandatory mbit, Protected pbit);

}
}

#endif /* AVP_H_ */
