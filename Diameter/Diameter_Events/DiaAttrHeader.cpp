/*
 * DiaAttrHeader.cpp
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#include "DiaAttrHeader.h"

namespace Diameter {
namespace Attr {

Dia_Attr_Header_raw::Dia_Attr_Header_raw(integer applicationId, integer commandCode) :
        m_version {1},
        m_length {0},
        m_command_Code {commandCode},
        m_application_ID {applicationId},
        m_end_To_EndIdentifier {0},
        m_hop_By_HopIdentifier {0},
        m_commandFlags {0} {
}

Dia_Attr_Header_raw::~Dia_Attr_Header_raw() {
}

integer Dia_Attr_Header_raw::getApplicationId() const {
    return m_application_ID;
}

void Dia_Attr_Header_raw::setApplicationId(integer applicationId) {
    m_application_ID = applicationId;
}

integer Dia_Attr_Header_raw::getCommandCode() const {
    return m_command_Code;
}

void Dia_Attr_Header_raw::setCommandCode(integer commandCode) {
    m_command_Code = commandCode;
}

integer Dia_Attr_Header_raw::getEndToEndIdentifier() const {
    return m_end_To_EndIdentifier;
}

void Dia_Attr_Header_raw::setEndToEndIdentifier(integer endToEndIdentifier) {
    m_end_To_EndIdentifier = endToEndIdentifier;
}

bool Dia_Attr_Header_raw::getErrorFlag() const {
    return (m_commandFlags & ERROR_FLAG_MASK) > 0;
}

void Dia_Attr_Header_raw::setErrorFlag(bool errorFlag) {
    if (errorFlag)
        m_commandFlags |= ERROR_FLAG_MASK;
    else
        m_commandFlags &= ~ERROR_FLAG_MASK;
}

integer Dia_Attr_Header_raw::getHopByHopIdentifier() const {
    return m_hop_By_HopIdentifier;
}

void Dia_Attr_Header_raw::setHopByHopIdentifier(integer hopByHopIdentifier) {
    m_hop_By_HopIdentifier = hopByHopIdentifier;
}

integer Dia_Attr_Header_raw::getLength() const {
    return m_length;
}

void Dia_Attr_Header_raw::setLength(integer length) {
    m_length = length;
}

bool Dia_Attr_Header_raw::getProxiableFlag() const {
    return (m_commandFlags & PROXIABLE_FLAG_MASK) > 0;
}

void Dia_Attr_Header_raw::setProxiableFlag(bool proxiableFlag) {
    if (proxiableFlag)
        m_commandFlags |= PROXIABLE_FLAG_MASK;
    else
        m_commandFlags &= ~PROXIABLE_FLAG_MASK;
}

bool Dia_Attr_Header_raw::getRequestFlag() const {
    return (m_commandFlags & REQUEST_FLAG_MASK) > 0;
}

void Dia_Attr_Header_raw::setRequestFlag(bool requestFlag) {
    if (requestFlag)
        m_commandFlags |= REQUEST_FLAG_MASK;
    else
        m_commandFlags &= ~REQUEST_FLAG_MASK;
}

bool Dia_Attr_Header_raw::getRetransmitFlag() const {
    return (m_commandFlags & RETRANSMIT_FLAG_MASK) > 0;
}

void Dia_Attr_Header_raw::setRetransmitFlag(bool retransmitFlag) {
    if (retransmitFlag)
       m_commandFlags |= RETRANSMIT_FLAG_MASK;
    else
        m_commandFlags &= ~RETRANSMIT_FLAG_MASK;
}

integer Dia_Attr_Header_raw::getVersion() const {
    return m_version;
}

void Dia_Attr_Header_raw::setVersion(integer version) {
    m_version = version;
}

Dia_Attr_Header make_dia_attr_header(integer applicationId, integer commandCode) {
    return std::make_shared<Dia_Attr_Header_raw>(applicationId, commandCode);
}

std::uint8_t Dia_Attr_Header_raw::getCommandFlags() const {
    return m_commandFlags;
}
} /* namespace Attr */
} /* namespace Diameter */

