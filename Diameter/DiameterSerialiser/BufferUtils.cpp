/*
 * BufferUtils.cpp
 *
 *  Created on: 7 Aug 2016
 *      Author: alanmc
 */

#include "BufferUtils.h"

#include <iostream>

namespace Diameter {
namespace Events {

size_t pad(size_t inputSize) {
    size_t mod = (inputSize % sizeof(std::uint32_t));
    if (mod > 0) {
        return inputSize + ((sizeof(std::uint32_t) - mod));
    }
    return inputSize;
}

size_t avpSize(std::uint32_t vendorId, size_t valueSize) {
     using Serialiser::internal::AVP;   
     size_t sz = ((vendorId == 0) ? (sizeof(AVP) + valueSize) :
            (sizeof(AVP) + sizeof(std::uint32_t) + valueSize));

    return sz;
}

}
}

