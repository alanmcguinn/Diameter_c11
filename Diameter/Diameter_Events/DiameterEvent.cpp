/*
 * DiameterEvent.cpp
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#include "DiameterEvent.h"
#include <iostream>

namespace Diameter {
namespace Events {

Diameter_Event::Diameter_Event(integer applicationId, integer commandCode, bool request) {
	m_header = Attr::make_dia_attr_header(applicationId, commandCode);
	m_header->setRequestFlag(request);
}

Diameter_Event::~Diameter_Event() {
}

Attr::Dia_Attr_Header Diameter_Event::getHeader() {
    return m_header;
}

Diameter_Base_Ptr makeNone() {
	return Diameter_Base_Ptr(nullptr);
}

} /* namespace Events */
} /* namespace Diameter */
