set(_target DiameterEvents)

set(srcFiles DiaAttrHeader.cpp DiameterEvent.cpp)

add_library(${_target} SHARED ${srcFiles})

install(TARGETS ${_target} DESTINATION sdk/lib)
install(FILES DiaAttrTypes.h DiaAttrHeader.h DiameterEvent.h traits.h DESTINATION sdk/includes/${_target})

set_property(TARGET ${_target} PROPERTY VERSION 1.0.0.0)
set_property(TARGET ${_target} PROPERTY SOVERSION 1.0)
set_property(TARGET ${_target} PROPERTY CXX_STANDARD 14)

