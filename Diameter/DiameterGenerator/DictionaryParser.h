/*
 * DictionaryParser.h
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#ifndef DICTIONARYPARSER_H_
#define DICTIONARYPARSER_H_

#include <string>
#include <functional>
#include <boost/any.hpp>

#include "Application.h"
#include "Avp.h"
#include "Command.h"

namespace Diameter {
namespace Generator {

class DictionaryParser {
public:
    virtual ~DictionaryParser() {}

    virtual const boost::any& parse(const std::string&) = 0;
    virtual boost::any getBase() = 0;

    virtual void for_each_application(std::function<void(Application&)> f) = 0;
    virtual void for_each_avp(const boost::any& ctx, std::function<void(Avp&)> f) = 0;
    virtual void for_each_command(const boost::any& ctx, std::function<void(Command&)> f) = 0;

    virtual std::pair<std::string, std::string> getType(const std::string& type) const = 0;
};

}
}

#endif /* DICTIONARYPARSER_H_ */
