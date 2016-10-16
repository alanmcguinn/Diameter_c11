/*
 * Request.cpp
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#include "Message.h"

namespace Diameter {
namespace Dictionary {

Message::Message(std::function<Diameter::Events::Diameter_Base_Ptr()> eventCreator) :
	m_eventCreator(eventCreator) {
}

Message::~Message() {
}

void Message::addFixedRule(CommandEntryBase commandEntry) {
    addRule(m_fixedRules, commandEntry);
}

void Message::addRequiredRule(CommandEntryBase commandEntry) {
    addRule(m_requiredRules, commandEntry);
}

void Message::addOptionalRule(CommandEntryBase commandEntry) {
    addRule(m_optionalRules, commandEntry);
}

void Message::addRule(std::vector<CommandEntryBase>& ruleList, CommandEntryBase& commandEntry) {
    ruleList.push_back(commandEntry);
    m_commandEntries.insert(std::make_pair(commandEntry->getCommandCode(), commandEntry));
}

Diameter::Events::Diameter_Base_Ptr Message::make() {
	return m_eventCreator();
}

} /* namespace Dictionary */
} /* namespace Diameter */
