/*
 * Header_s.h
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#ifndef HEADER_S_H_
#define HEADER_S_H_

#include <cstdint>
#include <arpa/inet.h>
#include <iostream>

#include "Utils_s.h"

namespace Diameter {
namespace Serialiser {
namespace internal {

struct Header_s {
	std::uint32_t versionLength;
	std::uint32_t commandFlagsCode;
	std::uint32_t applicationId;
	std::uint32_t hopByHopIdentifier;
	std::uint32_t endToEndIdentifier;

	void setVersionLength(std::uint8_t version, std::uint32_t length) {
		setField(versionLength, version, length);
	}

	std::uint8_t getVersion() const {
		return getUpperByte(versionLength);
	}

	std::uint32_t getLength() const {
		return getLower24Bytes(versionLength);
	}

	void setCommandFlagsCode(std::uint8_t flags, std::uint32_t code) {
		setField(commandFlagsCode, flags, code);
	}

	std::uint8_t getCommandFlags() const {
		return getUpperByte(commandFlagsCode);
	}

	std::uint32_t getCommandCode() const {
		return getLower24Bytes(commandFlagsCode);
	}

	bool isRequest() const {
		std::uint8_t commandFlags = getCommandFlags();
		return (commandFlags & 0x80) > 0;
	}

	void setApplicationId(std::uint32_t applicationId) {
		this->applicationId = htonl(applicationId);
	}

	std::uint32_t getApplicationId() const {
		return ntohl(applicationId);
	}

	void setHopByHopIdentifier(std::uint32_t hopByHopIdentifier) {
		this->hopByHopIdentifier = htonl(hopByHopIdentifier);
	}

	std::uint32_t getHopByHopIdentifier() const {
		return ntohl(hopByHopIdentifier);
	}

	void setEndToEndIdentifier(std::uint32_t endToEndIdentifier) {
		this->endToEndIdentifier = htonl(endToEndIdentifier);
	}

	std::uint32_t getEntToEndIdentifier() const {
		return ntohl(endToEndIdentifier);
	}
};

}
}
}

#endif /* HEADER_S_H_ */
