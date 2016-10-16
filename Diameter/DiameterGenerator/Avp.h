/*
 * Avp.h
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#ifndef AVP_H_
#define AVP_H_

#include <string>
#include <cstdint>
#include <functional>

#include <boost/any.hpp>

#include "GroupOfAvps.h"

namespace Diameter {
namespace Generator {

class Avp : public GroupOfAvps {
public:
    Avp(boost::any ctx, const std::string name, std::uint32_t code, std::string mandatory, std::string may_encrypt,
            std::string _protected, std::uint32_t vendor_id, std::string type, bool isGrouped);
    virtual ~Avp();

    // non-copyable and non-assignable
    Avp(const Avp&) = delete;
    Avp& operator=(const Avp&) = delete;

    Avp(Avp&& copy);
    Avp& operator=(Avp&& rhs);

    std::uint32_t getCode() const;
    const boost::any& getCtx() const;
    const std::string& getMandatory() const;
    const std::string& getMayEncrypt() const;
    const std::string& getName() const;
    const std::string& getProtected() const;
    const std::string& getType() const;
    std::uint32_t getVendorId() const;

    bool isGrouped() const;
    bool isEnumerated() const;

    void addEnumeration(const std::string& enumerationName, std::uint32_t enumerationValue);

    void for_each_enumeration(std::function<void(std::pair<std::string, std::uint32_t>&)> f);

private:
    boost::any m_ctx;
    std::string m_name;
    std::uint32_t m_code;
    std::string m_mandatory;
    std::string m_may_encrypt;
    std::string m_protected;
    std::uint32_t m_vendor_id;

    std::string m_type;

    bool m_isGrouped;

    std::vector<std::pair<std::string, std::uint32_t>> m_enumeration;

};

} /* namespace Generator */
} /* namespace Diameter */

#endif /* AVP_H_ */
