/*
 * GroupedAVP.cpp
 *
 *  Created on: 24 Jul 2016
 *      Author: alanmc
 */

#include "GroupedAVP.h"

namespace Diameter {
namespace Dictionary {

GroupedAVPRaw::GroupedAVPRaw(const std::string& commandName, std::uint32_t commandCode,
		std::function<Diameter::Events::Diameter_Base_Ptr()> attrCreator) :
	Message(attrCreator), m_name(commandName), m_code(commandCode) {
}

GroupedAVPRaw::~GroupedAVPRaw() {
}

const std::string& GroupedAVPRaw::getName() const {
    return m_name;
}

std::uint32_t GroupedAVPRaw::getCode() const {
    return m_code;
}

GroupedAVP makeGroupedAVP(const std::string& name, std::uint32_t code,
		std::function<Diameter::Events::Diameter_Base_Ptr()> attrCreator) {
    return std::make_shared<GroupedAVPRaw>(name, code, attrCreator);
}


} /* namespace Dictionary */
} /* namespace Diameter */
