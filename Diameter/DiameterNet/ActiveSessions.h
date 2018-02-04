//
// Created by alanmc on 04/02/18.
//

#ifndef DIAMETER_ACTIVESESSIONS_H
#define DIAMETER_ACTIVESESSIONS_H

#include "Session.h"

#include <map>
#include <memory>
#include <mutex>

namespace Diameter {
namespace Net {

class ActiveSessions {
public:

    void add(std::shared_ptr<Session> session);
    void remove(int key);

private:
    std::mutex m_lock;
    std::map<int, std::shared_ptr<Session>> m_activeSessions;
};

}
}

#endif //DIAMETER_ACTIVESESSIONS_H
