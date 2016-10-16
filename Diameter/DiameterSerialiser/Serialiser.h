/*
 * Serialiser.h
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#ifndef SERIALISER_H_
#define SERIALISER_H_

#include <vector>
#include <cstdint>

#include <type_traits>

#include "Header_s.h"
#include "AVP_s.h"
#include "DiaAttrHeader.h"
#include "DictionaryDefinition.h"
#include "Buffer.h"
#include "BufferUtils.h"
#include <exception>

namespace Diameter {
namespace Serialiser {

void addToBuffer(Buffer& buffer, internal::Header_s& header);

class Serialiser {
public:
    Serialiser() = delete;

    template <typename Event>
    static Event deserialise(Diameter::Dictionary::DictionaryDefinition& dictionary, Buffer& buffer) {
    	internal::Header_s* headerStruct = buffer.get<internal::Header_s>(sizeof(internal::Header_s));

        using Diameter::Dictionary::Message;
        using Diameter::Dictionary::CommandEntryBase;

    	Message& message = dictionary.getMessage(headerStruct->getCommandCode(), headerStruct->isRequest());

    	Event event = message.makeEvent<Event>();

        populateHeader(*headerStruct, event->getHeader());

        internal::AVP* avp { nullptr };
        do {
        
            avp = buffer.get<internal::AVP>(sizeof(internal::AVP));
            if (avp != nullptr) {
                std::cout << "AVP code is " << ntohl(avp->m_tag);
                std::cout << ", AVP size is " << avp->getLength() << std::endl;
                buffer.get<char>(avp->getLength() - sizeof(internal::AVP));

                message.applyToCommandEntry(avp->getCommandCode(), [&event, avp, &dictionary](CommandEntryBase& commandEntry) {
                    commandEntry->deserialise(event, avp->m_value, avp->getLength(), dictionary);
                });
            }
        } while (avp != nullptr);

    	if (!event) {
    		std::cout << " Could not create event" << std::endl;
    	} else {
    		std::cout << " Created event" << std::endl;
    	}

        return event;
    }

    template <typename Event>
    static Buffer serialise(Diameter::Dictionary::DictionaryDefinition& dictionary, Event event) {
        using Diameter::Attr::Dia_Attr_Header;

        Buffer buffer(8192);

        internal::Header_s* headerStruct = buffer.reserve<internal::Header_s>(sizeof(internal::Header_s));
        Dia_Attr_Header header = event->getHeader();

        populateHeader(header, *headerStruct);

        using Diameter::Dictionary::Message;
        using Diameter::Dictionary::CommandEntryBase;

        Message& message = dictionary.getMessage(getType(event));

        size_t sz = sizeof(internal::Header_s);

        message.for_each_fixed_rule([&event, &buffer, &dictionary, &sz](CommandEntryBase& commandEntry) {
            sz += commandEntry->serialise(event, buffer, dictionary);
        });
        message.for_each_required_rule([&event, &buffer, &dictionary, &sz](CommandEntryBase& commandEntry) {
            sz += commandEntry->serialise(event, buffer, dictionary);
        });
        message.for_each_optional_rule([&event, &buffer, &dictionary, &sz](CommandEntryBase& commandEntry) {
            sz += commandEntry->serialise(event, buffer, dictionary);
        });

        headerStruct->setVersionLength(1, sz);

        return buffer;
    }

private:
    static void populateHeader(const Attr::Dia_Attr_Header& source, internal::Header_s& dest);
    static void populateHeader(const internal::Header_s& source, Attr::Dia_Attr_Header&& dest); 
};

} /* namespace Serialiser */
} /* namespace Diameter */

#endif /* SERIALISER_H_ */
