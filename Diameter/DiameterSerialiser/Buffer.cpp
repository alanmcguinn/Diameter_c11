/*
 * Buffer.cpp
 *
 *  Created on: 1 Aug 2016
 *      Author: alanmc
 */

#include "Buffer.h"

#include <iomanip>
#include <boost/smart_ptr/make_shared_array.hpp>
#include <boost/io/ios_state.hpp>

namespace Diameter {
namespace Serialiser {


Buffer::Buffer(size_t initialSize) : m_buffer {new unsigned char[initialSize]}, m_initialSize {initialSize},
        m_size {0}, m_numBytesLeft {initialSize}, m_currentWrite {m_buffer.get()}, m_currentRead {m_buffer.get()} {
}

Buffer::Buffer(Buffer&& copy) : m_buffer {std::move(copy.m_buffer)}, m_initialSize {std::move(copy.m_initialSize)},
        m_size {std::move(copy.m_size)}, m_numBytesLeft {std::move(copy.m_numBytesLeft)},
        m_currentWrite {std::move(copy.m_currentWrite)}, m_currentRead {std::move(copy.m_currentRead)} {
}

void Buffer::reset() {
	m_currentWrite = m_buffer.get();
	m_numBytesLeft = m_initialSize;
	m_size = 0;
}

Buffer& Buffer::operator=(Buffer&& rhs) {
    m_buffer = std::move(rhs.m_buffer);
    m_initialSize = std::move(rhs.m_initialSize);
    m_size = std::move(rhs.m_size);
    m_numBytesLeft = std::move(rhs.m_numBytesLeft);
    m_currentWrite = std::move(rhs.m_currentWrite);
    m_currentRead = std::move(rhs.m_currentRead);

    return *this;
}

void Buffer::resize(size_t size) {
    if (m_numBytesLeft < size) {
        // We need a resize
        size_t resizeSize = std::max(m_size + m_numBytesLeft + m_initialSize,
                m_size + m_numBytesLeft + size);
        boost::shared_array<unsigned char> tmp(new unsigned char[resizeSize]);
        m_numBytesLeft += std::max(m_initialSize, size);

        // Perform the copy
        for (size_t i = 0; i < m_size; ++i) {
            tmp[i] = m_buffer[i];
        }

        m_currentWrite = &tmp[m_size];

        m_buffer = tmp;
    }
}

std::ostream& operator<<(std::ostream& os, Buffer& buffer) {
    boost::io::ios_flags_saver flagSaver(os);

    os << std::hex << std::setfill('0');

    unsigned long address = 0;

    size_t offset = 0;

    while (offset < buffer.size()) {
        os << std::setw(8) << address;
        for (size_t j = offset; j < (offset + 16); ++j) {
            if (j >= buffer.size()) {
                os << "   ";
            } else {
                unsigned char c = (unsigned char)(buffer.m_buffer[j]);
                os << ' ' << std::setw(2) << (unsigned short)c;
            }
            address++;
        }

        os << "    ";
        for (size_t j = offset; j < (offset + 16); ++j) {
            if (j >= buffer.size()) {
                break;
            }
            if ((buffer.m_buffer[j] < 32) || (buffer.m_buffer[j] > 127))
                os << '.';
            else
                os << buffer.m_buffer[j];
        }

        offset += 16;
        os << std::endl;
    }

    return os;
}


} /* namespace Serialiser */
} /* namespace Diameter */
