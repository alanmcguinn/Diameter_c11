/*
 * MessageGenerator.h
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#ifndef MESSAGEGENERATOR_H_
#define MESSAGEGENERATOR_H_

#include "../Config.h"
#include "../DictionaryParser.h"
#include "../Generator.h"
#include "../Avp.h"

#include <unordered_map>
#include <sstream>

namespace Diameter {
namespace Generator {

class MessageGenerator : public Generator {
public:
    MessageGenerator(Config& config);
    virtual ~MessageGenerator();

    void generate(DictionaryParser& dictParser) override;

private:

    void seedAVPs(DictionaryParser& dictParser);
    void generateAVPs(DictionaryParser& dictParser);
    void generateEvents(DictionaryParser& dictParser);
    void generateDictionary(DictionaryParser& dictParser);

    std::string normalise(std::string str);
    std::string getType(DictionaryParser& dictParser, const Avp& avp);
    std::string getConverter(DictionaryParser& dictParser, const Avp& avp);

    void generateEvent(DictionaryParser& dictParser, Application& application, Command& command);
    void generateEnumeratedAVP(Avp& avp, const std::string& branding);
    void generateGroupedAVP(DictionaryParser& dictParser, const std::string& branding, Avp& avp);

    template <typename T>
    void generateAttributes(DictionaryParser& dictParser, T& obj, std::unordered_map<std::string, std::string>& templateMap) {
        std::stringstream attributeStream;
        std::stringstream includesStream;
        std::stringstream gettersStream;
        std::stringstream settersStream;
        std::stringstream initialiserListStream;

        using std::placeholders::_1;
        auto populateMethod = std::bind(&MessageGenerator::populateAttributeStream, this, std::ref(dictParser), _1,
                std::ref(attributeStream), std::ref(includesStream), std::ref(gettersStream), std::ref(settersStream),
                std::ref(initialiserListStream));

        attributeStream << TAB << "// Fixed Attributes " << std::endl;
        obj.for_each_fixed_avp(populateMethod);

        attributeStream << TAB << "// Required Attributes " << std::endl;
        obj.for_each_required_avp(populateMethod);

        attributeStream << TAB << "// Optional Attributes " << std::endl;
        obj.for_each_optional_avp(populateMethod);

        templateMap["%Attributes%"] = attributeStream.str();
        templateMap["%Includes%"] = includesStream.str();
        templateMap["%AttributeGetters%"] = gettersStream.str();
        templateMap["%AttributeSetters%"] = settersStream.str();
        templateMap["%InitialiserList%"] = (initialiserListStream.str().size() > 2) ? initialiserListStream.str() : "";
    }

    void populateAttributeStream(DictionaryParser& dictParser, const std::string& avpName,
            std::stringstream& attributeStream, std::stringstream& enumIncludesStream,
            std::stringstream& gettersStream, std::stringstream& settersStream,
            std::stringstream& initialiserListStream);
    void populateEnumerations(Avp& avp, std::stringstream& enumStream);
    void populateDictionaryAvpStream(DictionaryParser& dictParser, std::stringstream& dictionaryAVPs,
            std::stringstream& dictionaryGroupedAVPs,
            std::stringstream& dictionaryIncludes, std::stringstream& avpStringLiterals);
    void populateDictionaryGroupedAVPStream(DictionaryParser& dictParser, Avp& avp, std::stringstream& dictionaryAVPs);
    void populateDictionaryCommandsStream(DictionaryParser& dictParser, Application& application,
            std::stringstream& dictionaryCommands, std::stringstream& dictionaryIncludes);
    void populateMakeCommandEntry(DictionaryParser& dictParser, Command& command, std::stringstream& dictionaryCommands,
            const std::string& commandType, std::string ruleType, const std::string& avpName);
    void populateMakeGroupedAVPEntry(DictionaryParser& dictParser, Avp& command, std::stringstream& dictionaryCommands,
            std::string ruleType, const std::string& avpName);
    Config& m_config;
    std::unordered_map<std::string, Avp> m_avpMap;

private:
    static const std::string TAB;
};

} /* namespace Generator */
} /* namespace Diameter */

#endif /* MESSAGEGENERATOR_H_ */
