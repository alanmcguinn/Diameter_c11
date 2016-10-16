/*
 * Avp.cpp
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#include "Avp.h"

namespace Diameter {
namespace Generator {

Avp::Avp(boost::any ctx, const std::string name, std::uint32_t code, std::string mandatory, std::string may_encrypt,
        std::string _protected, std::uint32_t vendor_id, std::string type, bool isGrouped) :
            m_ctx(ctx), m_name(name), m_code(code), m_mandatory(mandatory),
            m_may_encrypt(may_encrypt), m_protected(_protected), m_vendor_id(vendor_id),
            m_type(type), m_isGrouped(isGrouped) {
}

Avp::~Avp() {
}

Avp::Avp(Avp&& copy) : GroupOfAvps(std::move(copy)),
            m_ctx(std::move(copy.m_ctx)), m_name(std::move(copy.m_name)),
            m_code(std::move(copy.m_code)), m_mandatory(std::move(copy.m_mandatory)),
            m_may_encrypt(std::move(copy.m_may_encrypt)), m_protected(std::move(copy.m_protected)),
            m_vendor_id(std::move(copy.m_vendor_id)), m_type(std::move(copy.m_type)),
            m_isGrouped(std::move(copy.m_isGrouped)), m_enumeration(std::move(copy.m_enumeration)) {
}

Avp& Avp::operator =(Avp&& rhs) {
    GroupOfAvps::operator =(std::move(rhs));

    m_ctx = std::move(rhs.m_ctx);
    m_name = std::move(rhs.m_name);
    m_code = std::move(rhs.m_code);
    m_mandatory = std::move(rhs.m_mandatory);
    m_may_encrypt = std::move(rhs.m_may_encrypt);
    m_protected = std::move(rhs.m_protected);
    m_vendor_id = std::move(rhs.m_vendor_id);
    m_type = std::move(rhs.m_type);
    m_isGrouped = std::move(rhs.m_isGrouped);
    m_enumeration = std::move(rhs.m_enumeration);

    return *this;
}

std::uint32_t Avp::getCode() const {
    return m_code;
}

const boost::any& Avp::getCtx() const {
    return m_ctx;
}

const std::string& Avp::getMandatory() const {
    return m_mandatory;
}

const std::string& Avp::getMayEncrypt() const {
    return m_may_encrypt;
}

const std::string& Avp::getName() const {
    return m_name;
}

const std::string& Avp::getProtected() const {
    return m_protected;
}

const std::string& Avp::getType() const {
    return m_type;
}

std::uint32_t Avp::getVendorId() const {
    return m_vendor_id;
}

bool Avp::isGrouped() const {
    return m_isGrouped;
}

void Avp::addEnumeration(const std::string& enumerationName, std::uint32_t enumerationValue) {
    m_enumeration.push_back(std::make_pair(enumerationName, enumerationValue));
}

bool Avp::isEnumerated() const {
    return (m_enumeration.empty() == false);
}

void Avp::for_each_enumeration(std::function<void(std::pair<std::string, std::uint32_t> &)> f) {
    std::for_each(std::begin(m_enumeration), std::end(m_enumeration), f);
}

} /* namespace Generator */
} /* namespace Diameter */
