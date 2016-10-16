#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <string>
#include <cstdint>
#include <memory>

namespace Diameter {

namespace Dictionary {
class DictionaryDefinition;
}

namespace Serialiser {

std::shared_ptr<std::uint32_t> convertToUint32(Dictionary::DictionaryDefinition& dictionaryDefinition, char* buffer, std::uint32_t bufferSize);
std::shared_ptr<std::int32_t> convertToInt32(Dictionary::DictionaryDefinition& dictionaryDefinition, char* buffer, std::uint32_t bufferSize);
std::shared_ptr<std::uint64_t> convertToUint64(Dictionary::DictionaryDefinition& dictionaryDefinition, char* buffer, std::uint32_t bufferSize);
std::shared_ptr<std::int64_t> convertToInt64(Dictionary::DictionaryDefinition& dictionaryDefinition, char* buffer, std::uint32_t bufferSize);
std::string convertToString(Dictionary::DictionaryDefinition& dictionaryDefinition, char* buffer, std::uint32_t bufferSize);

}
}


#endif /* CONVERTERS_H */
