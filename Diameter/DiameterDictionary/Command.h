/*
 * Command.h
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
#include <cstdint>
#include <memory>

#include "Message.h"

namespace Diameter {
namespace Dictionary {

class CommandRaw {
public:
    CommandRaw(const std::string& commandName, std::uint32_t commandCode,
    		std::function<Diameter::Events::Diameter_Base_Ptr()> requestCreator,
    		std::function<Diameter::Events::Diameter_Base_Ptr()> answerCreator);
    virtual ~CommandRaw();

    const std::string& getCommandName() const;
    std::uint32_t getCommandCode() const;
    Message& getRequest();
    Message& getAnswer();

private:
    std::string m_commandName;
    std::uint32_t m_commandCode;

    Message m_request;
    Message m_answer;
};

using Command = std::shared_ptr<CommandRaw>;

Command makeCommand(const std::string& commandName, std::uint32_t commandCode,
		std::function<Diameter::Events::Diameter_Base_Ptr()> requestCreator,
		std::function<Diameter::Events::Diameter_Base_Ptr()> answerCreator);

} /* namespace Dictionary */
} /* namespace Diameter */

#endif /* COMMAND_H_ */
