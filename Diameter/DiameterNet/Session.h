//
// Created by alanmc on 04/02/18.
//

#ifndef DIAMETER_SESSION_H
#define DIAMETER_SESSION_H

#include "Buffer.h"

#include <functional>
#include <memory>
#include <boost/asio.hpp>

namespace Diameter {
namespace Net {

using boost::asio::io_service;
using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(io_service& io, tcp::socket& socket, std::function<void()> onClose);

    int key();
private:

    void performRead();

    Serialiser::Buffer m_buffer;

    io_service& m_io;
    tcp::socket m_socket;
};

}
}

#endif //DIAMETER_SESSION_H
