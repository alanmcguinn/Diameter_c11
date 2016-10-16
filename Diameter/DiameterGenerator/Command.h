/*
 * Command.h
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <boost/any.hpp>
#include <string>
#include <cstdint>
#include <functional>

#include "GroupOfAvps.h"

namespace Diameter {
namespace Generator {

class Command : public GroupOfAvps {
public:
    Command(boost::any ctx, const std::string& rootName, const std::string& name, const std::uint32_t code, std::uint32_t pbit, bool answerOnly = false);
    virtual ~Command();

    // non-copyable and non-assignable
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    Command(Command&& copy);
    Command& operator=(Command& rhs);

    std::uint32_t getCode() const;
    const boost::any& getCtx() const;
    const std::string& getRootName() const;
    const std::string& getName() const;
    std::uint32_t getPbit() const;

    bool isAnswerOnly() const;

private:

    boost::any m_ctx;
    std::string m_rootName;
    std::string m_name;
    std::uint32_t m_code;
    std::uint32_t m_pbit;

    bool m_answerOnly;
};

} /* namespace Generator */
} /* namespace Diameter */

#endif /* COMMAND_H_ */
