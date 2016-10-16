/*
 * DictionaryEntry.h
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#ifndef COMMANDENTRY_H_
#define COMMANDENTRY_H_

#include <cstdint>
#include <memory>
#include "AVP.h"

#include "DiameterEvent.h"


#include "Buffer.h"

namespace Diameter {
namespace Dictionary {

class DictionaryDefinition;

class CommandEntryBaseRaw {
public:
    virtual ~CommandEntryBaseRaw() {}

    virtual size_t serialise(Diameter::Events::Diameter_Base_Ptr obj,
            Diameter::Serialiser::Buffer& buffer, DictionaryDefinition& dictionary) = 0;
    virtual void deserialise(Diameter::Events::Diameter_Base_Ptr obj, char* buffer,
            std::uint32_t bufferSize, DictionaryDefinition& dictionary) = 0;
    virtual std::uint32_t getCommandCode() = 0;
};

template <typename ObjType, typename AttrType, typename Getter, typename Setter, typename Converter>
class CommandEntry : public CommandEntryBaseRaw {
public:
    CommandEntry(AVP avp, Getter getter, Setter setter, Converter converter)
            : m_avp{avp}, m_getter{getter} , m_setter {setter}, m_converter(converter) {
    }

    virtual ~CommandEntry() {}

private:

    AVP m_avp;
    Getter m_getter;
    Setter m_setter;
    Converter m_converter;

public:

    auto performGet(ObjType* obj) -> decltype((obj->*m_getter)()) {
        return (obj->*m_getter)();
    }

    void performSet(ObjType* obj, AttrType attr) {
        (obj->*m_setter)(attr);
    }

    AVP getAVP() const {
        return m_avp;
    }

    std::uint32_t getCommandCode() override {
            return m_avp->getAvpCode();
    }

    size_t serialise(Diameter::Events::Diameter_Base_Ptr obj, Diameter::Serialiser::Buffer& buffer,
            DictionaryDefinition& dictionary) override {
        typename std::shared_ptr<ObjType> qualifiedObj = std::static_pointer_cast<ObjType>(obj);
        using namespace Diameter::Events;

        size_t sz = 0;

        auto attribute = performGet(qualifiedObj.get());
        if (isSet(attribute)) {
            sz += addToBuffer(buffer, dictionary, m_avp->getAvpCode(), m_avp->getVendorId(), attribute);
        }
        return sz;
    }

    void deserialise(Diameter::Events::Diameter_Base_Ptr obj, char* buffer,
            std::uint32_t bufferSize, DictionaryDefinition& dictionary) override {
        typename std::shared_ptr<ObjType> qualifiedObj = std::static_pointer_cast<ObjType>(obj);

        performSet(qualifiedObj.get(), m_converter(dictionary, buffer, bufferSize));
    }

};

using CommandEntryBase = std::shared_ptr<CommandEntryBaseRaw>;

template <typename ObjType, typename AttrType, typename Getter, typename Setter, typename Converter = int>
CommandEntryBase makeCommandEntry(AVP avp, Getter getter, Setter setter, Converter converter = 0) {
    auto retVal = std::make_shared<CommandEntry<ObjType, AttrType, Getter, Setter, Converter>>(avp, getter, setter, converter);

    return retVal;
}

} /* namespace Dictionary */
} /* namespace Diameter */

#endif /* COMMANDENTRY_H_ */
