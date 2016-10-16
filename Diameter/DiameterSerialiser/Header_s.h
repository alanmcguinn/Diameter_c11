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
};

}
}
}

#endif /* HEADER_S_H_ */
