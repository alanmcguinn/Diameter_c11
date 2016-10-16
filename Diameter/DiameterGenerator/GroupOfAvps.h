/*
 * GroupOfAVps.h
 *
 *  Created on: 16 Jul 2016
 *      Author: alanmc
 */

#ifndef GROUPOFAVPS_H_
#define GROUPOFAVPS_H_

#include <vector>
#include <string>

#include <functional>

namespace Diameter {
namespace Generator {

class GroupOfAvps {
public:
    GroupOfAvps();
    virtual ~GroupOfAvps();

    // non-copyable and non-assignable
    GroupOfAvps(const GroupOfAvps&) = delete;
    GroupOfAvps& operator=(const GroupOfAvps&) = delete;

    GroupOfAvps(GroupOfAvps&& copy);
    GroupOfAvps& operator=(GroupOfAvps&& rhs);

    void addFixedAvp(const std::string& avpName);
    void addRequiredAvp(const std::string& avpName);
    void addOptionalAvp(const std::string& avpName);

    void for_each_fixed_avp(std::function<void(const std::string& avpName)> f);
    void for_each_required_avp(std::function<void(const std::string& avpName)> f);
    void for_each_optional_avp(std::function<void(const std::string& avpName)> f);


private:

    void addAvp(std::vector<std::string>& avpList, const std::string& avpName);

    std::vector<std::string> m_fixedAvps;
    std::vector<std::string> m_requiredAvps;
    std::vector<std::string> m_optionalAvps;

};

} /* namespace Generator */
} /* namespace Diameter */

#endif /* GROUPOFAVPS_H_ */
