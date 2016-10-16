/*
 * Command.cpp
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#include "Command.h"

namespace Diameter {
namespace Dictionary {

CommandRaw::CommandRaw(const std::string& commandName, std::uint32_t commandCode,
		std::function<Diameter::Events::Diameter_Base_Ptr()> requestCreator,
		std::function<Diameter::Events::Diameter_Base_Ptr()> answerCreator)
        : m_commandName(commandName), m_commandCode(commandCode),
		  m_request(requestCreator), m_answer(answerCreator) {
}

CommandRaw::~CommandRaw() {
}

Message& CommandRaw::getAnswer() {
    return m_answer;
}

std::uint32_t CommandRaw::getCommandCode() const {
    return m_commandCode;
}

const std::string& CommandRaw::getCommandName() const {
    return m_commandName;
}

Message& CommandRaw::getRequest() {
    return m_request;
}

Command makeCommand(const std::string& commandName, std::uint32_t commandCode,
		std::function<Diameter::Events::Diameter_Base_Ptr()> requestCreator,
		std::function<Diameter::Events::Diameter_Base_Ptr()> answerCreator) {
    return std::make_shared<CommandRaw>(commandName, commandCode, requestCreator, answerCreator);
}


} /* namespace Dictionary */
} /* namespace Diameter */
