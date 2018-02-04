//
// Created by alanmc on 04/02/18.
//

#include "Endpoint.h"

namespace Diameter {
namespace Net {

Endpoint::Endpoint(io_service& io) :
        m_io(io),
        m_acceptor(io, tcp::endpoint(tcp::v4(), DEFAULT_PORT)) {
    performAccept();
}

void Endpoint::performAccept() {
    tcp::socket socket(m_io);
    m_acceptor.async_accept(socket, [this, &socket](boost::system::error_code ec) {
        if (!ec) {
            m_activeSessions.add(std::make_shared<Session>(m_io, socket, [this, &socket]() {
                m_activeSessions.remove(socket.native_handle());
            }));
        }
        performAccept();
    });
}

}
}