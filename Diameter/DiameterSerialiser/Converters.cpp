#include "Converters.h"

namespace Diameter {
namespace Serialiser {
 
std::string convertToString(Dictionary::DictionaryDefinition& dictionaryDefinition, char* buffer, std::uint32_t bufferSize) {
    return std::string(buffer, bufferSize);
}

std::shared_ptr<std::uint32_t> convertToUint32(Dictionary::DictionaryDefinition& dictionaryDefinition, char* buffer, std::uint32_t bufferSize) {
    std::uint32_t* uintPtr = reinterpret_cast<std::uint32_t*>(buffer);

    return std::make_shared<std::uint32_t>(*uintPtr);
}

std::shared_ptr<std::int32_t> convertToInt32(Dictionary::DictionaryDefinition& dictionaryDefinition, char* buffer, std::uint32_t bufferSize) {
    std::int32_t* intPtr = reinterpret_cast<std::int32_t*>(buffer);

    return std::make_shared<std::int32_t>(*intPtr);
}

std::shared_ptr<std::uint64_t> convertToUint64(Dictionary::DictionaryDefinition& dictionaryDefinition, char* buffer, std::uint32_t bufferSize) {
    std::uint64_t* uintPtr = reinterpret_cast<std::uint64_t*>(buffer);

    return std::make_shared<std::uint64_t>(*uintPtr);
}

std::shared_ptr<std::int64_t> convertToInt64(Dictionary::DictionaryDefinition& dictionaryDefinition, char* buffer, std::uint32_t bufferSize) {
    std::int64_t* uintPtr = reinterpret_cast<std::int64_t*>(buffer);

    return std::make_shared<std::int64_t>(*uintPtr);
}

}
}

