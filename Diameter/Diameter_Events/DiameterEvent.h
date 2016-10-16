/*
 * DiameterEvent.h
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#ifndef DIAMETEREVENT_H_
#define DIAMETEREVENT_H_

#include "DiaAttrHeader.h"

#include <memory>
#include <vector>
#include <cstdint>
#include <type_traits>

namespace Diameter {
namespace Events {

class Diameter_Base {};

class Diameter_Event : public Diameter_Base {
public:
    Diameter_Event(integer applicationId, integer commandCode, bool request);
    virtual ~Diameter_Event();

    Attr::Dia_Attr_Header getHeader();

private:
    Attr::Dia_Attr_Header m_header;

};

using Diameter_Base_Ptr = std::shared_ptr<Diameter_Base>;
using Diameter_Event_Ptr = std::shared_ptr<Diameter_Event>;

Diameter_Base_Ptr makeNone();

} /* namespace Events */
} /* namespace Diameter */

#endif /* DIAMETEREVENT_H_ */
