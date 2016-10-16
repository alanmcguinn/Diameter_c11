/*
 * GroupOfAVps.cpp
 *
 *  Created on: 16 Jul 2016
 *      Author: alanmc
 */

#include "GroupOfAvps.h"

#include <iostream>
#include <algorithm>

namespace Diameter {
namespace Generator {

GroupOfAvps::GroupOfAvps() {
}

GroupOfAvps::~GroupOfAvps() {
}

void GroupOfAvps::addFixedAvp(const std::string& avpName) {
    std::cout << "Adding Fixed Avp " << avpName << std::endl;
    addAvp(m_fixedAvps, avpName);
}

void GroupOfAvps::addRequiredAvp(const std::string& avpName) {
    std::cout << "Adding Required Avp " << avpName << std::endl;
    addAvp(m_requiredAvps, avpName);
}

void GroupOfAvps::addOptionalAvp(const std::string& avpName) {
    std::cout << "Adding Optional Avp " << avpName << std::endl;
    addAvp(m_optionalAvps, avpName);
}

void GroupOfAvps::for_each_fixed_avp(std::function<void(const std::string& avpName)> f) {
    std::for_each(std::begin(m_fixedAvps), std::end(m_fixedAvps), f);
}

void GroupOfAvps::for_each_required_avp(std::function<void(const std::string& avpName)> f) {
    std::for_each(std::begin(m_requiredAvps), std::end(m_requiredAvps), f);
}

void GroupOfAvps::for_each_optional_avp(std::function<void(const std::string& avpName)> f) {
    std::for_each(std::begin(m_optionalAvps), std::end(m_optionalAvps), f);
}

GroupOfAvps::GroupOfAvps(GroupOfAvps&& copy) : m_fixedAvps(std::move(copy.m_fixedAvps)),
        m_requiredAvps(std::move(copy.m_requiredAvps)), m_optionalAvps(std::move(copy.m_optionalAvps)) {
}

GroupOfAvps& GroupOfAvps::operator =(GroupOfAvps&& rhs) {
    m_fixedAvps = std::move(rhs.m_fixedAvps);
    m_requiredAvps = std::move(rhs.m_requiredAvps);
    m_optionalAvps = std::move(rhs.m_optionalAvps);

    return *this;
}

void GroupOfAvps::addAvp(std::vector<std::string>& avpList, const std::string& avpName) {
    avpList.push_back(avpName);
}

} /* namespace Generator */
} /* namespace Diameter */
