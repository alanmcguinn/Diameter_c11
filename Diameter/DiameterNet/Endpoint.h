//
// Created by alanmc on 04/02/18.
//

#ifndef DIAMETER_ENDPOINT_H
#define DIAMETER_ENDPOINT_H

#include "Session.h"
#include "ActiveSessions.h"

#include <map>
#include <boost/asio.hpp>

namespace Diameter {
namespace Net {

using boost::asio::ip::tcp;
using boost::asio::io_service;

class Endpoint {
public:
    Endpoint(io_service& io);
private:

    void performAccept();

    io_service& m_io;
    tcp::acceptor m_acceptor;

    ActiveSessions m_activeSessions;

    static const int DEFAULT_PORT = 3868;


};

}
}

#endif //DIAMETER_ENDPOINT_H
