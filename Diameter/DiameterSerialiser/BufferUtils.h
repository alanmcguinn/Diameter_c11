#ifndef _BUFFER_UTILS_H
#define _BUFFER_UTILS_H

#include <cstdint>
#include <vector>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <cstring>
#include <arpa/inet.h>

#include "Buffer.h"
#include "AVP_s.h"

#include "DiameterEvent.h"

#include "traits.h"

namespace Diameter {

namespace Dictionary {

class DictionaryDefinition;

}

namespace Events {

template <typename T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
bool isSet(T& t) {
    return (t != T::UNSET);
}

template <typename T, typename std::enable_if<is_shared_ptr<T>::value, int>::type = 0>
bool isSet(const T& t) {
    return (t.get() != nullptr);
}

template <typename T, typename std::enable_if<std::is_same<T, std::string>::value, int>::type = 0>
bool isSet(const T& t) {
    return !t.empty();
}

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
bool isSet(T& t) {
    return t != 0;
}

template <typename T, typename std::enable_if<!is_shared_ptr<T>::value, int>::type = 0>
std::type_index getType(T& t) {
    return typeid(t);
}

template <typename T, typename std::enable_if<is_shared_ptr<T>::value, int>::type = 0>
std::type_index getType(T& t) {
    return typeid(*t);
}

template <typename T, typename std::enable_if<is_shared_ptr<T>::value, int>::type = 0>
size_t size(const T& i) {
    return sizeof(typename T::element_type);
}

size_t pad(size_t inputSize);

template <typename T, typename std::enable_if<std::is_same<T, std::string>::value, int>::type = 0>
size_t size(const T& str) {
    return pad(str.size());
}

template <typename T, typename std::enable_if<std::is_same<T, std::string>::value, int>::type = 0>
void doCopy(Serialiser::internal::AVP* avp, const T& val, int offset = 0) {
    std::memcpy(&avp->m_value[offset], val.c_str(), val.size());
}

template <typename T, typename std::enable_if<is_shared_ptr<T>::value, int>::type = 0>
void doCopy(Serialiser::internal::AVP* avp, const T& val, int offset = 0) {
    std::memcpy(&avp->m_value[offset], val.get(), size(val));
}

size_t avpSize(std::uint32_t vendorId, size_t valueSize);

template <typename T, typename std::enable_if<!std::is_enum<T>::value && !is_diameter_base_shared_ptr<T>::value, int>::type = 0>
size_t addToBuffer(Serialiser::Buffer& buffer, Dictionary::DictionaryDefinition& dctionary,
        std::uint32_t avpCode, std::uint32_t vendorId, const T& i) {
    using Serialiser::internal::AVP;
    size_t avpLength = avpSize(vendorId, size(i));
    AVP* avp = buffer.reserve<AVP>(avpLength);
    avp->m_tag = htonl(avpCode);
    int offset = avp->setVendorId(vendorId);
    avp->m_length = avpLength;
    doCopy(avp, i, offset);

    return avpLength;
}

template <typename T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
size_t addToBuffer(Serialiser::Buffer& buffer, Dictionary::DictionaryDefinition& dictionary, std::uint32_t code,
        std::uint32_t vendorId, T& val) {
    using Serialiser::internal::AVP;

    AVP* avp = buffer.reserve<AVP>(avpSize(vendorId, sizeof(std::uint32_t)));

    std::uint32_t enumVal = htonl(std::uint32_t(val));
    
    size_t avpLength = sizeof(AVP) + sizeof(std::uint32_t);
    avp->m_tag = htonl(code);
    avp->m_length = avpLength;

    int offset = avp->setVendorId(vendorId);

    std::uint32_t otwEnumVal = htonl(enumVal);

    std::memcpy(&avp->m_value[offset], &otwEnumVal, sizeof(std::uint32_t));

    return avpLength;
}

}
}

#endif /* _BUFFER_UTILS_H */
