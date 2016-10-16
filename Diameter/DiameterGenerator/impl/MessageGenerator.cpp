/*
 * MessageGenerator.cpp
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#include "MessageGenerator.h"
#include "../Application.h"
#include "TemplateApplicator.h"

#include <boost/algorithm/string/predicate.hpp>

#include <iostream>

namespace Diameter {
namespace Generator {

const std::string MessageGenerator::TAB { "    " };

MessageGenerator::MessageGenerator(Config& config) :
        m_config(config) {
}

MessageGenerator::~MessageGenerator() {
}

void MessageGenerator::generate(DictionaryParser& dictParser) {
    // Seed the AVP Database
    seedAVPs(dictParser);

    generateAVPs(dictParser);

    generateEvents(dictParser);

    generateDictionary(dictParser);
}

void MessageGenerator::seedAVPs(DictionaryParser& dictParser) {
    dictParser.for_each_avp(dictParser.getBase(), [this](Avp& avp) {
        m_avpMap.insert(std::make_pair(avp.getName(), std::move(avp)));
    });

    dictParser.for_each_application([&dictParser, this](Application& application) {
        dictParser.for_each_avp(application.getCtx(), [&dictParser, &application, this](Avp& avp) {
            m_avpMap.insert(std::make_pair(avp.getName(), std::move(avp)));
        });
    });
}

void MessageGenerator::generateAVPs(DictionaryParser& dictParser) {
    if (m_config.willGenerateBaseAVPs()) {
        dictParser.for_each_avp(dictParser.getBase(), [&dictParser, this](Avp& avp) {
            if (avp.isEnumerated()) {
                generateEnumeratedAVP(avp, "Base");
            } else if (avp.isGrouped()) {
                generateGroupedAVP(dictParser, "Base", avp);
            }
        });
    }

    dictParser.for_each_application([&dictParser, this](Application& application) {
        dictParser.for_each_avp(application.getCtx(), [&dictParser, &application, this](Avp& avp) {
            if (avp.isEnumerated()) {
                generateEnumeratedAVP(avp, application.getName());
            }
        });
    });

    dictParser.for_each_application([&dictParser, this](Application& application) {
        dictParser.for_each_avp(application.getCtx(), [&dictParser, &application, this](Avp& avp) {
            if (avp.isGrouped()) {
                generateGroupedAVP(dictParser, application.getName(), avp);
            }
        });
    });
}

void MessageGenerator::generateEvents(DictionaryParser& dictParser) {
    Application baseApplication(dictParser.getBase(), 0, "", "", "Base", "");
    dictParser.for_each_command(dictParser.getBase(), [&dictParser, &baseApplication, this](Command& command) {
        generateEvent(dictParser, baseApplication, command);
    });

    dictParser.for_each_application([&dictParser, this](Application& application) {
        dictParser.for_each_command(application.getCtx(), [&dictParser, &application, this](Command& command) {
            generateEvent(dictParser, application, command);
        });
    });
}

void MessageGenerator::generateDictionary(DictionaryParser& dictParser) {
    auto dictCommandsHeaderGen = [&dictParser, this](Application& application) {
        std::unordered_map<std::string, std::string> templateMap;
        templateMap["%Branding%"] = application.getName();

        TemplateApplicator templateApplicator(m_config.getTemplateDir(), "Dictionary_h.tmpl", m_config.getOutputDir());
        templateApplicator.applyTemplate(templateMap, normalise(application.getName())+ "_Dictionary.h");
    };

    if (m_config.willGenerateBaseAVPs()) {
        Application baseApplication(dictParser.getBase(), 0, "", "", "Base", "");
        dictCommandsHeaderGen(baseApplication);
    }
    dictParser.for_each_application(dictCommandsHeaderGen);

    auto dictCommandsGen = [&dictParser, this](Application& application) {
        std::stringstream dictionaryIncludes;
        std::stringstream dictionaryAVPs;
        std::stringstream dictionaryGroupedAVPs;
        std::stringstream avpStringLiterals;
        populateDictionaryAvpStream(dictParser, dictionaryAVPs, dictionaryGroupedAVPs, dictionaryIncludes, avpStringLiterals);

        std::stringstream dictionaryCommands;
        populateDictionaryCommandsStream(dictParser, application, dictionaryCommands, dictionaryIncludes);

        std::unordered_map<std::string, std::string> templateMap;
        templateMap["%Branding%"] = application.getName();
        templateMap["%AVPStringLiterals%"] = avpStringLiterals.str();
        templateMap["%AVPs%"] = dictionaryAVPs.str();
        templateMap["%GroupedAVPs%"] = dictionaryGroupedAVPs.str();
        templateMap["%Commands%"] = dictionaryCommands.str();
        templateMap["%Includes%"] = dictionaryIncludes.str();

        TemplateApplicator templateApplicator(m_config.getTemplateDir(), "Dictionary_cpp.tmpl", m_config.getOutputDir());
        templateApplicator.applyTemplate(templateMap, normalise(application.getName())+ "_Dictionary.cpp");
    };

    if (m_config.willGenerateBaseAVPs()) {
        Application baseApplication(dictParser.getBase(), 0, "", "", "Base", "");
        dictCommandsGen(baseApplication);
    }
    dictParser.for_each_application(dictCommandsGen);
}



void MessageGenerator::generateEnumeratedAVP(Avp& avp, const std::string& branding) {
    std::unordered_map<std::string, std::string> templateMap;
    std::stringstream enumValuesStream;
    populateEnumerations(avp, enumValuesStream);
    templateMap["%Branding%"] = branding;
    templateMap["%EnumName%"] = normalise(avp.getName());
    templateMap["%EnumValues%"] = enumValuesStream.str();

    static const std::string DIA_ENUM("DiaEnum_");

    TemplateApplicator templateApplicatorHeader(m_config.getTemplateDir(), "DiameterEnum_h.tmpl", m_config.getOutputDir());
    templateApplicatorHeader.applyTemplate(templateMap, normalise(DIA_ENUM + avp.getName()) + ".h");

    TemplateApplicator templateApplicatorSource(m_config.getTemplateDir(), "DiameterEnum_cpp.tmpl", m_config.getOutputDir());
    templateApplicatorSource.applyTemplate(templateMap, normalise(DIA_ENUM + avp.getName()) + ".cpp");
}

void MessageGenerator::generateGroupedAVP(DictionaryParser& dictParser, const std::string& branding, Avp& avp) {
    std::unordered_map<std::string, std::string> templateMap;

    static const std::string DIA_ENUM("DiaAttr_");

    templateMap["%Branding%"] = branding;
    templateMap["%AttrName%"] = normalise(avp.getName());

    generateAttributes(dictParser, avp, templateMap);

    TemplateApplicator templateApplicatorHeader(m_config.getTemplateDir(), "DiameterAttr_h.tmpl", m_config.getOutputDir());
    templateApplicatorHeader.applyTemplate(templateMap, DIA_ENUM + normalise(avp.getName()) + ".h");

    TemplateApplicator templateApplicatorSource(m_config.getTemplateDir(), "DiameterAttr_cpp.tmpl", m_config.getOutputDir());
    templateApplicatorSource.applyTemplate(templateMap, DIA_ENUM + normalise(avp.getName()) + ".cpp");
}

void MessageGenerator::generateEvent(DictionaryParser& dictParser, Application& application, Command& command) {
    using boost::algorithm::ends_with;

    std::unordered_map<std::string, std::string> templateMap;
    templateMap["%Branding%"] = application.getName();
    templateMap["%ApplicationId%"] = std::to_string(application.getApplicationId());
    templateMap["%CommandCode%"] = std::to_string(command.getCode());
    templateMap["%MessageName%"] = normalise(command.getName());
    templateMap["%Request%"] = (ends_with(command.getName(), "Request") ? "true" : "false");

    generateAttributes(dictParser, command, templateMap);

    TemplateApplicator templateApplicator(m_config.getTemplateDir(), "DiameterEvent_h.tmpl", m_config.getOutputDir());
    templateApplicator.applyTemplate(templateMap, normalise(command.getName()) + ".h");

}

void MessageGenerator::populateAttributeStream(DictionaryParser& dictParser, const std::string& avpName,
        std::stringstream& attributeStream, std::stringstream& includesStream,
        std::stringstream& gettersStream, std::stringstream& settersStream,
        std::stringstream& initialiserListStream) {
    auto result = m_avpMap.find(avpName);
    std::string type = getType(dictParser, result->second);

    std::string attrName = normalise(result->second.getName());

    attributeStream << TAB << type << " m_" << attrName << ";" << std::endl;
    gettersStream << TAB;
    settersStream << TAB << "void set_" << attrName << "(";

    if (result->second.isEnumerated()) {
        gettersStream << type;
        settersStream << type;
        includesStream << "#include \"" << "DiaEnum_" << attrName << ".h\"" << std::endl;
        initialiserListStream <<"," << std::endl << TAB << TAB << "m_" << attrName << "(" << type << "::UNSET)";
    } else if (result->second.isGrouped()) {
        gettersStream << type;
        settersStream << type;
        includesStream << "#include \"" << "DiaAttr_" << attrName << ".h\"" << std::endl;
    } else {
        gettersStream << "const " << type << "&";
        settersStream <<  "const " << type << "&";
    }

    gettersStream << " get_" << attrName << "() const { " << std::endl;
      gettersStream << TAB << TAB << "return m_" << attrName << ";" << std::endl;
    gettersStream << TAB << "}" << std::endl << std::endl;

    settersStream << " " << attrName << ") {" << std::endl;
      settersStream << TAB << TAB << "m_" << attrName << " = " << attrName << ";" << std::endl;
    settersStream << TAB << "}" << std::endl << std::endl;
}

void MessageGenerator::populateEnumerations(Avp& avp, std::stringstream& enumStream) {
    avp.for_each_enumeration([&enumStream, this](std::pair<std::string, std::uint32_t>& enumEntry) {
        enumStream << "E_" << normalise(enumEntry.first) << " = " << enumEntry.second << ", ";
    });
}

std::string MessageGenerator::normalise(std::string str) {
    std::replace(std::begin(str), std::end(str), '-', '_');
    std::replace(std::begin(str), std::end(str), ' ', '_');
    std::replace(std::begin(str), std::end(str), '/', '_');
    return str;
}

std::string MessageGenerator::getType(DictionaryParser& dictParser, const Avp& avp) {
    std::stringstream type;
    if (avp.isGrouped()) {
        type << "std::shared_ptr<DiaAttr_" << normalise(avp.getName()) << ">";
    } else if (avp.isEnumerated()) {
        type << "DiaEnum_" << normalise(avp.getName());
    } else {
        type << dictParser.getType(avp.getType()).first;
    }

    return type.str();
}

std::string MessageGenerator::getConverter(DictionaryParser& dictParser, const Avp& avp) {
    std::stringstream converter;
    converter << "&Diameter::Serialiser::convertTo";
    if (avp.isGrouped()) {
        converter << "DiaAttr_" << normalise(avp.getName());
    } else if (avp.isEnumerated()) {
        converter << "DiaEnum_" << normalise(avp.getName());
    } else {
        converter << dictParser.getType(avp.getType()).second;
    }

    return converter.str();
}

void MessageGenerator::populateDictionaryAvpStream(DictionaryParser& dictParser, std::stringstream& dictionaryAVPs,
	std::stringstream& dictionaryGroupedAVPs,
        std::stringstream& dictionaryIncludes, std::stringstream& avpStringLiterals) {

    auto func = [&dictParser, &dictionaryAVPs, &dictionaryGroupedAVPs, &dictionaryIncludes, &avpStringLiterals, this](Avp& avp){

        avpStringLiterals << TAB << "auto strLit_" << normalise(avp.getName()) << " { \"" << avp.getName() << "\" };" << std::endl;

        std::string avpTemplate { TAB + "dictionary.addAVP(makeAVP(%%Name%%, %%Code%%, %%VendorId%%, Mandatory::%%MBit%%, Protected::%%PBit%%));" };
        TemplateApplicator::replaceAll(avpTemplate, "%%Name%%", std::string("strLit_") + normalise(avp.getName()));
        TemplateApplicator::replaceAll(avpTemplate, "%%Code%%", std::to_string(avp.getCode()));
        TemplateApplicator::replaceAll(avpTemplate, "%%VendorId%%", std::to_string(avp.getVendorId()));
        TemplateApplicator::replaceAll(avpTemplate, "%%MBit%%", avp.getMandatory());
        TemplateApplicator::replaceAll(avpTemplate, "%%PBit%%", avp.getProtected());
        dictionaryAVPs << avpTemplate << std::endl;

        if (avp.isGrouped()) {
            dictionaryIncludes << "#include \"DiaAttr_" << normalise(avp.getName()) << ".h\"" << std::endl;
            populateDictionaryGroupedAVPStream(dictParser, avp, dictionaryGroupedAVPs);
        }
    };

    dictParser.for_each_avp(dictParser.getBase(), func);
    dictParser.for_each_application([&dictParser, &dictionaryAVPs, &func](Application& application) {
        dictParser.for_each_avp(application.getCtx(), func);
    });
}

void MessageGenerator::populateDictionaryGroupedAVPStream(DictionaryParser& dictParser, Avp& avp, std::stringstream& dictionaryAVPs) {

    std::string makeGroupedAVPStr { "GroupedAVP %%GroupedAVPVariableName%% = makeGroupedAVP(%%AVPName%%, %%Code%%, %%Creator%%);" };

    TemplateApplicator::replaceAll(makeGroupedAVPStr, "%%AVPName%%", std::string("strLit_") + normalise(avp.getName()));
    TemplateApplicator::replaceAll(makeGroupedAVPStr, "%%GroupedAVPVariableName%%", normalise(avp.getName()));
    TemplateApplicator::replaceAll(makeGroupedAVPStr, "%%Code%%", std::to_string(avp.getCode()));
    TemplateApplicator::replaceAll(makeGroupedAVPStr, "%%Creator%%", std::string("makeDiaAttr_") + normalise(avp.getName()));
    dictionaryAVPs << TAB << makeGroupedAVPStr << std::endl;

    using std::placeholders::_1;

    auto populateFixedFunc = std::bind(&MessageGenerator::populateMakeGroupedAVPEntry, this, std::ref(dictParser), std::ref(avp), std::ref(dictionaryAVPs), "Fixed", _1);
    avp.for_each_fixed_avp(populateFixedFunc);

    auto populateRequiredFunc = std::bind(&MessageGenerator::populateMakeGroupedAVPEntry, this, std::ref(dictParser), std::ref(avp), std::ref(dictionaryAVPs), "Required", _1);
    avp.for_each_required_avp(populateRequiredFunc);

    auto populateOptionalFunc = std::bind(&MessageGenerator::populateMakeGroupedAVPEntry, this, std::ref(dictParser), std::ref(avp), std::ref(dictionaryAVPs), "Optional", _1);
    avp.for_each_optional_avp(populateOptionalFunc);

    std::string addGroupedAVPStr { "dictionary.addGroupedAVP(%%GroupedAVPVariableName%%, typeid(DiaAttr_%%AVPType%%));" };
    TemplateApplicator::replaceAll(addGroupedAVPStr, "%%GroupedAVPVariableName%%", normalise(avp.getName()));
    TemplateApplicator::replaceAll(addGroupedAVPStr, "%%AVPType%%", normalise(avp.getName()));
    dictionaryAVPs << TAB << addGroupedAVPStr << std::endl << std::endl;
}

void MessageGenerator::populateDictionaryCommandsStream(DictionaryParser& dictParser, Application& application,
        std::stringstream& dictionaryCommands, std::stringstream& dictionaryIncludes) {
    dictParser.for_each_command(application.getCtx(), [&dictParser, &dictionaryCommands, &dictionaryIncludes, &application, this](Command& command) {
        using boost::algorithm::ends_with;
        const std::string type = ((ends_with(command.getName(), "Request")) ? "Request" : "Answer");

        dictionaryIncludes << "#include \"" << normalise(command.getName()) << ".h\"" << std::endl;

        if ((type == "Request") || (command.isAnswerOnly())) {
            std::string makeCommandStr { "Command %%CommandVariableName%% = makeCommand(\"%%CommandName%%\", %%Code%%, %%RequestCreator%%, %%AnswerCreator%%);" };

            TemplateApplicator::replaceAll(makeCommandStr, "%%CommandName%%", command.getRootName());
            TemplateApplicator::replaceAll(makeCommandStr, "%%CommandVariableName%%", normalise(command.getRootName()));
            TemplateApplicator::replaceAll(makeCommandStr, "%%Code%%", std::to_string(command.getCode()));
            if (!command.isAnswerOnly())
            	TemplateApplicator::replaceAll(makeCommandStr, "%%RequestCreator%%", std::string("makeDia_") + normalise(command.getRootName()) + "Request");
            else
            	TemplateApplicator::replaceAll(makeCommandStr, "%%RequestCreator%%", "makeNone");
            TemplateApplicator::replaceAll(makeCommandStr, "%%AnswerCreator%%", std::string("makeDia_") + normalise(command.getRootName()) + "Answer");
            dictionaryCommands << TAB << makeCommandStr << std::endl;
        }

        using std::placeholders::_1;

        auto populateFixedFunc = std::bind(&MessageGenerator::populateMakeCommandEntry, this, std::ref(dictParser), std::ref(command), std::ref(dictionaryCommands), std::ref(type), "Fixed", _1);
        command.for_each_fixed_avp(populateFixedFunc);

        auto populateRequiredFunc = std::bind(&MessageGenerator::populateMakeCommandEntry, this, std::ref(dictParser), std::ref(command), std::ref(dictionaryCommands), std::ref(type), "Required", _1);
        command.for_each_required_avp(populateRequiredFunc);

        auto populateOptionalFunc = std::bind(&MessageGenerator::populateMakeCommandEntry, this, std::ref(dictParser), std::ref(command), std::ref(dictionaryCommands), std::ref(type), "Optional", _1);
        command.for_each_optional_avp(populateOptionalFunc);

        if (type == "Answer") {
            std::stringstream ss;
            ss << "dictionary.addCommand(%%CommandVariableName%%, typeid(";
            ss << (command.isAnswerOnly() ? "void" : "Dia_%%CommandType%%Request");
            ss << "), typeid(Dia_%%CommandType%%Answer));";
            std::string addCommandStr { ss.str() };
            TemplateApplicator::replaceAll(addCommandStr, "%%CommandVariableName%%", normalise(command.getRootName()));
            TemplateApplicator::replaceAll(addCommandStr, "%%CommandType%%", normalise(command.getRootName()));
            dictionaryCommands << TAB << addCommandStr << std::endl << std::endl;
        }
    });
}

void MessageGenerator::populateMakeGroupedAVPEntry(DictionaryParser& dictParser, Avp& avp, std::stringstream& dictionaryAVPs,
            std::string ruleType, const std::string& avpName) {
    auto result = m_avpMap.find(avpName);
    std::string type = getType(dictParser, result->second);
    std::string converter = getConverter(dictParser, result->second);

    std::string makeGroupedEntryStr { "%%AVPVariableName%%->add%%RuleType%%Rule(makeCommandEntry<DiaAttr_%%ClassType%%, %%AttrType%%>(dictionary.getAVP(%%AVPName%%), &DiaAttr_%%ClassType%%::get_%%VarName%%, &DiaAttr_%%ClassType%%::set_%%VarName%%, %%Converter%%));"};
    TemplateApplicator::replaceAll(makeGroupedEntryStr, "%%AVPVariableName%%", normalise(avp.getName()));
    TemplateApplicator::replaceAll(makeGroupedEntryStr, "%%RuleType%%", ruleType);
    TemplateApplicator::replaceAll(makeGroupedEntryStr, "%%AVPName%%", std::string("strLit_") + normalise(avpName));
    TemplateApplicator::replaceAll(makeGroupedEntryStr, "%%ClassType%%", normalise(avp.getName()));
    TemplateApplicator::replaceAll(makeGroupedEntryStr, "%%VarName%%", normalise(avpName));
    TemplateApplicator::replaceAll(makeGroupedEntryStr, "%%AttrType%%", type);
    TemplateApplicator::replaceAll(makeGroupedEntryStr, "%%Converter%%", converter);
    dictionaryAVPs << TAB << makeGroupedEntryStr << std::endl;
}


void MessageGenerator::populateMakeCommandEntry(DictionaryParser& dictParser, Command& command, std::stringstream& dictionaryCommands,
            const std::string& commandType, std::string ruleType, const std::string& avpName) {
    auto result = m_avpMap.find(avpName);
    std::string type = getType(dictParser, result->second);
    std::string converter = getConverter(dictParser, result->second);

    std::string makeCommandEntryStr { "%%CommandVariableName%%->get%%Type%%().add%%RuleType%%Rule(makeCommandEntry<Dia_%%ClassType%%, %%AttrType%%>(dictionary.getAVP(%%AVPName%%), &Dia_%%ClassType%%::get_%%VarName%%, &Dia_%%ClassType%%::set_%%VarName%%, %%Converter%%));"};
    TemplateApplicator::replaceAll(makeCommandEntryStr, "%%CommandVariableName%%", normalise(command.getRootName()));
    TemplateApplicator::replaceAll(makeCommandEntryStr, "%%RuleType%%", ruleType);
    TemplateApplicator::replaceAll(makeCommandEntryStr, "%%Type%%", commandType);
    TemplateApplicator::replaceAll(makeCommandEntryStr, "%%AVPName%%", std::string("strLit_") + normalise(avpName));
    TemplateApplicator::replaceAll(makeCommandEntryStr, "%%ClassType%%", normalise(command.getName()));
    TemplateApplicator::replaceAll(makeCommandEntryStr, "%%VarName%%", normalise(avpName));
    TemplateApplicator::replaceAll(makeCommandEntryStr, "%%AttrType%%", type);
    TemplateApplicator::replaceAll(makeCommandEntryStr, "%%Converter%%", converter);
    dictionaryCommands << TAB << makeCommandEntryStr << std::endl;
}

} /* namespace Generator */
} /* namespace Diameter */

