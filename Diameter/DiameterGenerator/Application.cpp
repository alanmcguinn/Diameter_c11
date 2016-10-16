/*
 * Application.cpp
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#include "Application.h"

namespace Diameter {
namespace Generator {

Application::Application(boost::any ctx, std::uint32_t applicationId, const std::string& type, const std::string& eventPrefix, const std::string& name, const std::string& lib) :
        m_ctx(ctx), m_applicationId(applicationId), m_type(type), m_eventPrefix(eventPrefix), m_name(name), m_lib(lib) {
}

Application::~Application() {
}

std::uint32_t Application::getApplicationId() const {
    return m_applicationId;
}

const boost::any& Application::getCtx() const {
    return m_ctx;
}

const std::string& Application::getEventPrefix() const {
    return m_eventPrefix;
}

const std::string& Application::getLib() const {
    return m_lib;
}

const std::string& Application::getName() const {
    return m_name;
}

Application::Application(Application&& copy) :
        m_ctx(std::move(copy.m_ctx)), m_applicationId(std::move(copy.m_applicationId)),
        m_type(std::move(copy.m_type)), m_eventPrefix(std::move(copy.m_eventPrefix)),
        m_name(std::move(copy.m_name)), m_lib(std::move(copy.m_lib)) {

}

Application& Application::operator=(Application& rhs) {
    m_ctx = std::move(rhs.m_ctx);
    m_applicationId = std::move(rhs.m_applicationId);
    m_type = std::move(rhs.m_type);
    m_eventPrefix = std::move(rhs.m_eventPrefix);
    m_name = std::move(rhs.m_name);
    m_lib = std::move(rhs.m_lib);

    return *this;
}

const std::string& Application::getType() const {
    return m_type;
}

} /* namespace Generator */
} /* namespace Diameter */
