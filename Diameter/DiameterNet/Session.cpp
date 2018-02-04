//
// Created by alanmc on 04/02/18.
//

#include "Session.h"

namespace Diameter {
namespace Net {

Session::Session(io_service& io, tcp::socket& socket, std::function<void()> onClose) :
        m_io(io), m_socket(std::move(socket)) {

}

void Session::performRead() {
    using boost::system::error_code;
    using namespace boost::system::errc;
    m_socket.async_read_some(boost::asio::buffer(m_buffer.buffer().get(), m_buffer.capacity()),
                                                  [this](error_code ec, std::size_t length) {
        if (ec == success) {
            m_buffer.reserve<char>(length);
            std::cout << "Read " << length << " bytes" << std::endl << m_buffer << std::endl;
        }
        if (ec != resource_unavailable_try_again) {
            m_socket.close();
        }
    });
}

int Session::key() {
    return m_socket.native_handle();
}

}
}