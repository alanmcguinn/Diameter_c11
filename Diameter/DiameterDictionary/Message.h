/*
 * Request.h
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cstdint>
#include <functional>

#include "CommandEntry.h"

namespace Diameter {
namespace Dictionary {

class Message {
public:
    Message(std::function<Diameter::Events::Diameter_Base_Ptr()> eventCreator);
    virtual ~Message();

    void addFixedRule(CommandEntryBase commandEntry);

    void addRequiredRule(CommandEntryBase commandEntry);

    void addOptionalRule(CommandEntryBase commandEntry);

    template <typename Func>
    void for_each_fixed_rule(Func f) {
        std::for_each(std::begin(m_fixedRules), std::end(m_fixedRules), f);
    }

    template <typename Func>
    void for_each_required_rule(Func f) {
        std::for_each(std::begin(m_requiredRules), std::end(m_requiredRules), f);
    }

    template <typename Func>
    void for_each_optional_rule(Func f) {
        std::for_each(std::begin(m_optionalRules), std::end(m_optionalRules), f);
    }

    template <typename Event>
    Event makeEvent() {
    	return std::static_pointer_cast<typename Event::element_type>(make());
    }

    template <typename Func>
    void applyToCommandEntry(std::uint32_t commandCode, Func f) {
        auto i = m_commandEntries.find(commandCode);
        if (i != std::end(m_commandEntries)) {
            f(i->second);
        }
    }

    Diameter::Events::Diameter_Base_Ptr make();

private:

    void addRule(std::vector<CommandEntryBase>& ruleList, CommandEntryBase& commandEntry);

    std::vector<CommandEntryBase> m_fixedRules;
    std::vector<CommandEntryBase> m_requiredRules;
    std::vector<CommandEntryBase> m_optionalRules;

    std::function<Diameter::Events::Diameter_Base_Ptr()> m_eventCreator;

    std::unordered_map<std::uint32_t, CommandEntryBase> m_commandEntries;
};

} /* namespace Dictionary */
} /* namespace Diameter */

#endif /* MESSAGE_H_ */
