/*
 * main.cpp
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <arpa/inet.h>

#define PACKED __attribute__((__packed__))

struct Header_s {
	std::uint32_t versionLength;
	std::uint32_t commandFlagsCode;


	void setVersionLength(std::uint8_t version, std::uint32_t length) {
		setField(versionLength, version, length);
	}

	std::uint8_t getVersion() {
		return getUpperByte(versionLength);
	}

	std::uint32_t getLength() {
		return getLower24Bytes(versionLength);
	}

	void setCommandFlagsCode(std::uint8_t flags, std::uint32_t code) {
		setField(commandFlagsCode, flags, code);
	}

	std::uint8_t getCommandFlags() {
		return getUpperByte(commandFlagsCode);
	}

	std::uint32_t getCommandCode() {
		return getLower24Bytes(commandFlagsCode);
	}

	static void setField(std::uint32_t& field, std::uint8_t version, std::uint32_t length) {
		std::uint32_t shiftedVersion = version;
		shiftedVersion <<= 24;
		shiftedVersion |= length;
		field = htonl(shiftedVersion);
	}

	static std::uint8_t getUpperByte(std::uint32_t field) {
		std::uint32_t hostOrderVersionLength = ntohl(field);
		hostOrderVersionLength &= 0xFF000000;
		hostOrderVersionLength >>= 24;
		return hostOrderVersionLength;
	}

	static std::uint32_t getLower24Bytes(std::uint32_t field) {
		std::uint32_t hostOrderVersionLength = ntohl(field);
		hostOrderVersionLength &= 0x00FFFFFF;
		return hostOrderVersionLength;
	}
};

int main() {

	Header_s test;

	test.setVersionLength(1, 1024);

	std::cout << "Version = " << std::dec << (int)(test.getVersion()) << std::endl;
	std::cout << "Length = " << std::dec << (int)(test.getLength()) << std::endl;

	//test.commandFlags =
	//test.commandCode = (htonl(1) << 4);

	std::cout << "Sizeof Test = " << sizeof(test) << std::endl;

return 0;
}


