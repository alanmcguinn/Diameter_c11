/*
 * Buffer.h
 *
 *  Created on: 1 Aug 2016
 *      Author: alanmc
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <memory>
#include <boost/shared_array.hpp>

#include <iostream>

namespace Diameter {
namespace Serialiser {

class Buffer {
public:
    explicit Buffer(size_t initialSize);

    Buffer(Buffer&& copy);
    Buffer(const Buffer&) = delete;

    Buffer& operator=(Buffer&& rhs);
    Buffer& operator=(const Buffer&) = delete;

    template <typename T>
    T* reserve(size_t size) {
        resize(size);

        T* retVal = reinterpret_cast<T*>(m_currentWrite);
        m_currentWrite += size;

        m_numBytesLeft -= size;
        m_size += size;

        return retVal;
    }

    template <typename T>
    T* get(size_t size) {
//        std::cout << "Current Write is " << (void*)m_currentWrite << ", Current Read is " << (void*)m_currentRead << ", Requested size is " << size << std::endl;
        if (size > (m_currentWrite - m_currentRead)) {
            return nullptr;
        }
        T* retVal = reinterpret_cast<T*>(m_currentRead);
        m_currentRead += size;

        return retVal;
    }

    size_t size() const {
        return m_size;
    }

    void reset();

private:

    void resize(size_t size);

    boost::shared_array<unsigned char> m_buffer;

    size_t m_initialSize;
    size_t m_size;
    size_t m_numBytesLeft;
    unsigned char* m_currentWrite;
    unsigned char* m_currentRead;

    friend std::ostream& operator<<(std::ostream& os, Buffer& buffer);
};

std::ostream& operator<<(std::ostream& os, Buffer& buffer);

} /* namespace Serialiser */
} /* namespace Diameter */

#endif /* BUFFER_H_ */
