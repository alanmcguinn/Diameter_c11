/*
 * RapidDictionaryParser.cpp
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#include "RapidDictionaryParser.h"
#include <boost/lexical_cast.hpp>

#include <iostream>

namespace Diameter {
namespace Generator {

const std::map<std::string, std::pair<std::string, std::string>> RapidDictionaryParser::TYPE_MAP {
    { "OctetString", { "std::string", "String" } },
    { "Address", { "std::string", "String" } },
    { "UTF8String", { "std::string", "String" } },
    { "Unsigned32", { "std::shared_ptr<std::uint32_t>", "Uint32" } },
    { "Unsigned64", { "std::shared_ptr<std::uint64_t>", "Uint64" } },
    { "Integer32", { "std::shared_ptr<std::int32_t>", "Int32" } },
    { "Integer64", { "std::shared_ptr<std::int64_t>", "Int64" } },
    { "DiameterURI", { "std::string", "String" } },
    { "DiameterIdentity", { "std::string", "String" } },
    { "IPFilterRule", { "std::string", "String" } },
    { "QOSFilterRule", { "std::string", "String" } },
    { "Any", { "std::string", "String"} },
};

RapidDictionaryParser::RapidDictionaryParser() {
}

RapidDictionaryParser::~RapidDictionaryParser() {
}

const boost::any& RapidDictionaryParser::parse(const std::string& dictionaryPath) {
    using namespace rapidxml;

    m_doc = std::make_shared<rapidxml::xml_document<>>();
    m_ctx = m_doc.get();

    m_xmlFile = std::make_shared<rapidxml::file<>>(dictionaryPath.c_str());

    m_doc->parse<0>(m_xmlFile->data());

    return m_ctx;
}

void RapidDictionaryParser::for_each_avp(const boost::any& ctx, std::function<void(Avp&)> f) {
    using namespace rapidxml;

    static const std::string AVP_TAG("avp");
    xml_node<>* parentNode = boost::any_cast<xml_node<>*>(ctx);

    for (xml_node<>* avpNode = parentNode->first_node(AVP_TAG.c_str());
            avpNode != nullptr; avpNode = avpNode->next_sibling(AVP_TAG.c_str())) {

        static const std::string TYPE_TAG("type");
        static const std::string GROUPED_TAG("grouped");
        static const std::string ENUM_TAG("enum");

        xml_node<>* type = avpNode->first_node(TYPE_TAG.c_str());
        xml_node<>* grouped = avpNode->first_node(GROUPED_TAG.c_str());

        bool isGrouped = (grouped != nullptr);

        Avp avp(avpNode,
                getAttribute(avpNode, "name"),
                boost::lexical_cast<std::uint32_t>(getAttribute(avpNode, "code", "0")),
                getAttribute(avpNode, "mandatory", "may"),
                getAttribute(avpNode, "may-encrypt", "may"),
                getAttribute(avpNode, "protected", "mustnot"),
                boost::lexical_cast<std::uint32_t>(getAttribute(avpNode, "vendor-id", "0")),
                getAttribute(type, "type-name", "std::string"),
                isGrouped);

        if (avp.getType() == "Enumerated") {
            for (xml_node<>* enumValue = avpNode->first_node(ENUM_TAG.c_str()); enumValue != nullptr;
                    enumValue = enumValue->next_sibling(ENUM_TAG.c_str())) {
                avp.addEnumeration(getAttribute(enumValue, "name"),
                        boost::lexical_cast<std::uint32_t>(getAttribute(enumValue, "code")));
            }
        } else if (isGrouped) {
            populateAVPs(grouped, avp);
        }

        f(avp);
    }
}

rapidxml::xml_node<>* RapidDictionaryParser::getDictionaryNode() {
    using namespace rapidxml;

    xml_node<>* rootNode = m_doc->first_node("dictionary");

    return rootNode;
}

void RapidDictionaryParser::for_each_application(std::function<void(Application&)> f) {
    using namespace rapidxml;

    static const std::string APPLICATION_TAG("application");

    for (xml_node<>* applicationNode = getDictionaryNode()->first_node(APPLICATION_TAG.c_str());
            applicationNode != nullptr;
            applicationNode = applicationNode->next_sibling(APPLICATION_TAG.c_str())) {

        Application app(applicationNode,
                boost::lexical_cast<std::uint32_t>(getAttribute(applicationNode, "id", "0")),
                getAttribute(applicationNode, "type"),
                getAttribute(applicationNode, "eventprefix"),
                getAttribute(applicationNode, "name"),
                getAttribute(applicationNode, "lib"));

        f(app);
    }
}

void RapidDictionaryParser::for_each_command(const boost::any& ctx, std::function<void(Command&)> f) {
    using namespace rapidxml;

    static const std::string COMMAND_TAG("command");
    static const std::string REQUEST_TAG("requestrules");
    static const std::string ANSWER_TAG("answerrules");

    xml_node<>* parentNode = boost::any_cast<xml_node<>*>(ctx);

    for (xml_node<>* commandNode = parentNode->first_node(COMMAND_TAG.c_str());
            commandNode != nullptr; commandNode = commandNode->next_sibling(COMMAND_TAG.c_str())) {

        Command request(commandNode,
                getAttribute(commandNode, "name"),
                getAttribute(commandNode, "name") + "Request",
                boost::lexical_cast<std::uint32_t>(getAttribute(commandNode, "code")),
                boost::lexical_cast<std::uint32_t>(getAttribute(commandNode, "pbit", "0")));

        xml_node<>* requestNode = commandNode->first_node(REQUEST_TAG.c_str());
        if (requestNode != nullptr) {
            populateAVPs(requestNode, request);
            f(request);
        }

        Command answer(commandNode,
                getAttribute(commandNode, "name"),
                getAttribute(commandNode, "name") + "Answer",
                boost::lexical_cast<std::uint32_t>(getAttribute(commandNode, "code")),
                boost::lexical_cast<std::uint32_t>(getAttribute(commandNode, "pbit", "0")),
                (requestNode == nullptr));

        xml_node<>* answerNode = commandNode->first_node(ANSWER_TAG.c_str());
        if (answerNode != nullptr) {
            populateAVPs(answerNode, answer);
            f(answer);
        }
    }
}

std::pair<std::string, std::string> RapidDictionaryParser::getType(const std::string& type) const {
    auto result = TYPE_MAP.find(type);
    if (result == std::end(TYPE_MAP)) {
        return std::make_pair<std::string, std::string>("Unknown", "Unknown");
    }
    return result->second;
}

std::string RapidDictionaryParser::getAttribute(rapidxml::xml_node<>* node, const std::string& attrName, const std::string& defaultVal) {
    using namespace rapidxml;
    if (node == nullptr)
        return defaultVal;
    xml_attribute<>* attr = node->first_attribute(attrName.c_str());
    if (attr == nullptr) {
        return defaultVal;
    }
    return attr->value();
}

boost::any RapidDictionaryParser::getBase() {
    using namespace rapidxml;
    static const std::string BASE_TAG("base");

    xml_node<>* baseNode = getDictionaryNode()->first_node(BASE_TAG.c_str());
    return baseNode;
}

} /* namespace Generator */
} /* namespace Diameter */

