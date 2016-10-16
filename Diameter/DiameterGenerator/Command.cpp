/*
 * Command.cpp
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#include "Command.h"
#include <iostream>

namespace Diameter {
namespace Generator {

Command::Command(boost::any ctx, const std::string& rootName, const std::string& name, const std::uint32_t code, std::uint32_t pbit, bool answerOnly) :
        m_ctx(ctx), m_rootName(rootName), m_name(name), m_code(code), m_pbit(pbit), m_answerOnly(answerOnly) {
}

Command::~Command() {
}

Command::Command(Command&& copy) : GroupOfAvps(std::move(copy)),
        m_ctx(std::move(copy.m_ctx)), m_name(std::move(copy.m_name)), m_code(std::move(copy.m_code)),
        m_pbit(std::move(copy.m_pbit)), m_answerOnly(std::move(copy.m_answerOnly)) {
}

Command& Command::operator =(Command& rhs) {
    GroupOfAvps::operator =(std::move(rhs));
    m_ctx = std::move(rhs.m_ctx);
    m_name = std::move(rhs.m_name);
    m_code = std::move(rhs.m_code);
    m_pbit = std::move(rhs.m_pbit);
    m_answerOnly = std::move(rhs.m_answerOnly);

    return *this;
}

std::uint32_t Command::getCode() const {
    return m_code;
}

const boost::any& Command::getCtx() const {
    return m_ctx;
}

const std::string& Command::getName() const {
    return m_name;
}

std::uint32_t Command::getPbit() const {
    return m_pbit;
}

const std::string& Command::getRootName() const {
    return m_rootName;
}

bool Command::isAnswerOnly() const {
    return m_answerOnly;
}

} /* namespace Generator */
} /* namespace Diameter */
