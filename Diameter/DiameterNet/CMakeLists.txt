set(_target DiameterNet)

set(srcFiles Endpoint.cpp Session.cpp ActiveSessions.cpp)

add_library(${_target} SHARED ${srcFiles})

include_directories(${Boost_INCLUDE_DIRS} ../DiameterSerialiser)

install(TARGETS ${_target} DESTINATION sdk/lib)
install(FILES
        Endpoint.h
        DESTINATION sdk/includes/${_target})

set_property(TARGET ${_target} PROPERTY VERSION 1.0.0.0)
set_property(TARGET ${_target} PROPERTY SOVERSION 1.0)
set_property(TARGET ${_target} PROPERTY CXX_STANDARD 14)

