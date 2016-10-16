/*
 * Application.h
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <boost/any.hpp>
#include <cstdint>
#include <string>

namespace Diameter {
namespace Generator {

class Application {
public:
    Application(boost::any ctx, std::uint32_t applicationId, const std::string& type, const std::string& eventPrefix, const std::string& name, const std::string& lib);
    virtual ~Application();

    // non-copyable and non-assignable
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&& copy);
    Application& operator=(Application& rhs);

    std::uint32_t getApplicationId() const;

    const boost::any& getCtx() const;

    const std::string& getEventPrefix() const;

    const std::string& getLib() const;

    const std::string& getName() const;

    const std::string& getType() const;

private:
    boost::any m_ctx;
    std::uint32_t m_applicationId;
    std::string m_type;
    std::string m_eventPrefix;
    std::string m_name;
    std::string m_lib;
};

} /* namespace Generator */
} /* namespace Diameter */

#endif /* APPLICATION_H_ */
