/*
 * DictionaryDefinition.h
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#ifndef DICTIONARYDEFINITION_H_
#define DICTIONARYDEFINITION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <typeindex>
#include "AVP.h"
#include "Command.h"
#include "GroupedAVP.h"
#include "CommandEntry.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <iostream>

namespace Diameter {
namespace Dictionary {

class DictionaryDefinition {
public:
    DictionaryDefinition();
    virtual ~DictionaryDefinition();

    DictionaryDefinition(const DictionaryDefinition&) = delete;
    DictionaryDefinition& operator=(const DictionaryDefinition& rhs) = delete;

    DictionaryDefinition(DictionaryDefinition&& copy);
    DictionaryDefinition& operator=(DictionaryDefinition&& rhs);

    void addAVP(AVP avp) {
        m_allAVPs.insert(avp);
    }

    void addCommand(Command command, std::type_index requestType, std::type_index answerType) {
        m_allCommands.insert(command);
        if (requestType != typeid(void)) {
           m_messages.emplace(requestType, command->getRequest());
        }
        m_messages.emplace(answerType, command->getAnswer());
    }

    void addGroupedAVP(GroupedAVP groupedAVP, std::type_index type) {
        m_allGroupedAVPs.insert(groupedAVP);
        m_messages.emplace(type, *groupedAVP);
    }

    AVP getAVP(const std::string& name);

    Message& getMessage(std::type_index messageType) const;

    Message& getMessage(std::uint32_t commandCode, bool isRequest);

private:

    using AVPMap = boost::multi_index_container<AVP,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::const_mem_fun<AVPRaw, const std::string&, &AVPRaw::getName> >,
            boost::multi_index::hashed_unique<
                boost::multi_index::composite_key<AVPRaw,
                   boost::multi_index::const_mem_fun<AVPRaw, std::uint32_t, &AVPRaw::getAvpCode>,
                   boost::multi_index::const_mem_fun<AVPRaw, std::uint32_t, &AVPRaw::getVendorId> > > > >;
    AVPMap m_allAVPs;

    using GroupedAVPMap = boost::multi_index_container<GroupedAVP,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::const_mem_fun<GroupedAVPRaw, const std::string&, &GroupedAVPRaw::getName> >,
            boost::multi_index::hashed_unique<
                boost::multi_index::const_mem_fun<GroupedAVPRaw, std::uint32_t, &GroupedAVPRaw::getCode> > > >;
    GroupedAVPMap m_allGroupedAVPs;

    using CommandMap = boost::multi_index_container<Command,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::const_mem_fun<CommandRaw, const std::string&, &CommandRaw::getCommandName> >,
            boost::multi_index::hashed_unique<
                boost::multi_index::const_mem_fun<CommandRaw, std::uint32_t, &CommandRaw::getCommandCode> > > >;
    CommandMap m_allCommands;

    std::unordered_map<std::type_index, Message&> m_messages;
};

} /* namespace Dictionary */
} /* namespace Diameter */

#endif /* DICTIONARYDEFINITION_H_ */
