/*
 * DiaAttrHeader.h
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#ifndef DIAATTRHEADER_H_
#define DIAATTRHEADER_H_

#include "DiaAttrTypes.h"
#include <memory>

namespace Diameter {
namespace Attr {

class Dia_Attr_Header_raw {
public:
    Dia_Attr_Header_raw(integer applicationId, integer commandCode);
    virtual ~Dia_Attr_Header_raw();

    integer getApplicationId() const;
    void setApplicationId(integer applicationId);

    integer getCommandCode() const;
    void setCommandCode(integer commandCode);

    integer getEndToEndIdentifier() const;
    void setEndToEndIdentifier(integer endToEndIdentifier);

    bool getErrorFlag() const;
    void setErrorFlag(bool errorFlag);

    integer getHopByHopIdentifier() const;
    void setHopByHopIdentifier(integer hopByHopIdentifier);

    integer getLength() const;
    void setLength(integer length);

    bool getProxiableFlag() const;
    void setProxiableFlag(bool proxiableFlag);

    bool getRequestFlag() const;
    void setRequestFlag(bool requestFlag);

    bool getRetransmitFlag() const;
    void setRetransmitFlag(bool retransmitFlag);

    integer getVersion() const;
    void setVersion(integer version);

    std::uint8_t getCommandFlags() const;

private:
    integer m_version;
    integer m_length;
    integer m_command_Code;
    integer m_application_ID;
    integer m_end_To_EndIdentifier;
    integer m_hop_By_HopIdentifier;

    std::uint8_t m_commandFlags;


    static const std::uint8_t REQUEST_FLAG_MASK = 0x80;
    static const std::uint8_t PROXIABLE_FLAG_MASK = 0x40;
    static const std::uint8_t ERROR_FLAG_MASK = 0x20;
    static const std::uint8_t RETRANSMIT_FLAG_MASK = 0x10;
};

using Dia_Attr_Header = std::shared_ptr<Dia_Attr_Header_raw>;

Dia_Attr_Header make_dia_attr_header(integer applicationId, integer commandCode);

} /* namespace Attr */
} /* namespace Diameter */

#endif /* DIAATTRHEADER_H_ */
