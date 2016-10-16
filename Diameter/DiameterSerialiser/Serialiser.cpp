/*
 * Serialiser.cpp
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#include "Serialiser.h"

namespace Diameter {
namespace Serialiser {

void Serialiser::populateHeader(const Attr::Dia_Attr_Header& source, internal::Header_s& dest) {
    dest.setVersionLength(source->getVersion(), source->getLength());
    dest.setCommandFlagsCode(source->getCommandFlags(), source->getCommandCode());
}

void Serialiser::populateHeader(const internal::Header_s& source, Attr::Dia_Attr_Header&& dest) {
    dest->setVersion(source.getVersion());
    dest->setLength(source.getLength());
    dest->setCommandCode(source.getCommandCode());
}

} /* namespace Serialiser */
} /* namespace Diameter */

