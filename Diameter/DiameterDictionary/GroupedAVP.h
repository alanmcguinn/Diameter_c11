/*
 * GroupedAVP.h
 *
 *  Created on: 24 Jul 2016
 *      Author: alanmc
 */

#ifndef GROUPEDAVP_H_
#define GROUPEDAVP_H_

#include <string>
#include <cstdint>
#include <memory>

#include "Message.h"

namespace Diameter {
namespace Dictionary {

class GroupedAVPRaw: public Message {
public:
    GroupedAVPRaw(const std::string& name, std::uint32_t code,
    		std::function<Diameter::Events::Diameter_Base_Ptr()> attrCreator);
    virtual ~GroupedAVPRaw();

    const std::string& getName() const;
    std::uint32_t getCode() const;

private:
    std::string m_name;
    std::uint32_t m_code;
};

using GroupedAVP = std::shared_ptr<GroupedAVPRaw>;

GroupedAVP makeGroupedAVP(const std::string& name, std::uint32_t code,
		std::function<Diameter::Events::Diameter_Base_Ptr()> attrCreator);

} /* namespace Dictionary */
} /* namespace Diameter */

#endif /* GROUPEDAVP_H_ */
