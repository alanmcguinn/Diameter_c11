/*
 * RapidDictionaryParser.h
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#ifndef RAPIDDICTIONARYPARSER_H_
#define RAPIDDICTIONARYPARSER_H_

#include "../DictionaryParser.h"
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#include <memory>
#include <map>

#include <boost/any.hpp>

namespace Diameter {
namespace Generator {

class RapidDictionaryParser: public DictionaryParser {
public:
	RapidDictionaryParser();
	virtual ~RapidDictionaryParser();

	virtual const boost::any& parse(const std::string&) override;
	virtual boost::any getBase() override;

	virtual void for_each_application(std::function<void(Application&)> f) override;
	virtual void for_each_avp(const boost::any& ctx, std::function<void(Avp&)> f) override;
	virtual void for_each_command(const boost::any& ctx, std::function<void(Command&)> f) override;

	virtual std::pair<std::string, std::string> getType(const std::string& type) const;
private:

	template <typename T>
	void populateAVPs(rapidxml::xml_node<>* node, T& obj) {

	    using TagsMap = std::vector<std::pair<std::string, std::function<void(T*, const std::string&)>>>;
	    using namespace rapidxml;

	    static const TagsMap tags {
	        { "fixed", &T::addFixedAvp },
	        { "required", &T::addRequiredAvp },
	        { "optional", &T::addOptionalAvp },
	    };

	    std::for_each(std::begin(tags), std::end(tags), [node, &obj, this](const typename TagsMap::value_type& entry) {
	        static const std::string AVPRULE_TAG("avprule");
	        xml_node<>* currentNode = node->first_node(entry.first.c_str());
	        if (currentNode) {
                for (xml_node<>* avpruleNode = currentNode->first_node(AVPRULE_TAG.c_str()); avpruleNode != nullptr;
                        avpruleNode = avpruleNode->next_sibling(AVPRULE_TAG.c_str())) {
                    entry.second(&obj, getAttribute(avpruleNode, "name"));
                }
	        }
	    });
	}


	rapidxml::xml_node<>* getDictionaryNode();

	std::string getAttribute(rapidxml::xml_node<>* node, const std::string& attrName, const std::string& defaultVal = "");

	std::string m_dictionaryPath;

	std::shared_ptr<rapidxml::file<>> m_xmlFile;
	std::shared_ptr<rapidxml::xml_document<>> m_doc;
	boost::any m_ctx;

	static const std::map<std::string, std::pair<std::string, std::string>> TYPE_MAP;
};

} /* namespace Generator */
} /* namespace Diameter */

#endif /* RAPIDDICTIONARYPARSER_H_ */
