/*
 * DictionaryDefinition.cpp
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#include "DictionaryDefinition.h"

namespace Diameter {
namespace Dictionary {

DictionaryDefinition::DictionaryDefinition() {
}

DictionaryDefinition::~DictionaryDefinition() {
}

DictionaryDefinition::DictionaryDefinition(DictionaryDefinition&& copy)
		: m_allAVPs(std::move(copy.m_allAVPs)), m_allGroupedAVPs(std::move(copy.m_allGroupedAVPs)),
		  m_allCommands(std::move(copy.m_allCommands)), m_messages(std::move(copy.m_messages)) {
}

DictionaryDefinition& DictionaryDefinition::operator=(DictionaryDefinition&& rhs) {
	m_allAVPs = std::move(rhs.m_allAVPs);
	m_allGroupedAVPs = std::move(rhs.m_allGroupedAVPs);
	m_allCommands = std::move(rhs.m_allCommands);

	m_messages = std::move(rhs.m_messages);

	return *this;
}

AVP DictionaryDefinition::getAVP(const std::string& name) {
    AVP retVal;
    auto& findIndex = m_allAVPs.get<0>();
    auto result = findIndex.find(name);
    if (result != std::end(findIndex)) {
        retVal = *result;
    }
    return retVal;
}

Message& DictionaryDefinition::getMessage(std::type_index messageType) const {
    auto retVal = m_messages.find(messageType);
    if (retVal != std::end(m_messages)) {
        return retVal->second;
    }
    throw std::runtime_error("Unknown message type");
}

Message& DictionaryDefinition::getMessage(std::uint32_t commandCode, bool isRequest) {
	auto& findIndex = m_allCommands.get<1>();
	auto result = findIndex.find(commandCode);
	if (result != std::end(findIndex)) {
		auto command = result->get();
		return (isRequest ? command->getRequest() : command->getAnswer());
	}
	throw std::runtime_error("Unknown Message type");
}

} /* namespace Dictionary */
} /* namespace Diameter */

