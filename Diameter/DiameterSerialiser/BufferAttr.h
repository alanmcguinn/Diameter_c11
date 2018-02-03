/*
 * BufferAttr.h
 *
 *  Created on: 8 Aug 2016
 *      Author: alanmc
 */

#ifndef BUFFERATTR_H_
#define BUFFERATTR_H_

#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

#include "Buffer.h"
#include "AVP_s.h"
#include "Message.h"
#include "CommandEntry.h"
#include "DictionaryDefinition.h"

#include "traits.h"

namespace Diameter {
namespace Events {

template <typename T, typename std::enable_if<is_diameter_base_shared_ptr<T>::value, int>::type = 0>
size_t addToBuffer(Serialiser::Buffer& buffer, Dictionary::DictionaryDefinition& dictionary, std::uint32_t code,
        std::uint32_t vendorId, const T& val) {
    using Serialiser::internal::AVP;

    size_t sz = (vendorId > 0) ? (sizeof(AVP) + sizeof(std::uint32_t)) :
            (sizeof(AVP));

    AVP* avp = buffer.reserve<AVP>(sz);
    avp->m_tag = htonl(code);
    avp->setVendorId(vendorId);

    using Dictionary::Message;
    using Dictionary::CommandEntryBase;

    std::cout << "Processing SubType " << getType(val).name() << std::endl;
    Message& message = dictionary.getMessage(getType(val));
    message.for_each_fixed_rule([&val, &buffer, &dictionary, &sz](CommandEntryBase& commandEntry) {
        sz += commandEntry->serialise(val, buffer, dictionary);
    });
    message.for_each_required_rule([&val, &buffer, &dictionary, &sz](CommandEntryBase& commandEntry) {
        sz += commandEntry->serialise(val, buffer, dictionary);
    });
    message.for_each_optional_rule([&val, &buffer, &dictionary, &sz](CommandEntryBase& commandEntry) {
        sz += commandEntry->serialise(val, buffer, dictionary);
    });

    avp->setFlagsLength(0, sz);
    return sz;
}

}
}

#endif /* BUFFERATTR_H_ */
