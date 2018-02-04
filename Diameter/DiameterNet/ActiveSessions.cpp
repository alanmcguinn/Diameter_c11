//
// Created by alanmc on 04/02/18.
//

#include "ActiveSessions.h"

namespace Diameter {
namespace Net {

void ActiveSessions::add(std::shared_ptr<Session> session) {
    std::lock_guard<std::mutex> l(m_lock);
    m_activeSessions[session->key()] = session;
}

void ActiveSessions::remove(int key) {
    std::lock_guard<std::mutex> l(m_lock);
    m_activeSessions.erase(key);
}

}
}