set(_target DiameterSerialiser)

set(srcFiles Buffer.cpp Serialiser.cpp BufferUtils.cpp Utils_s.cpp Converters.cpp)

file(GLOB sdkIncludes *.h)

add_library(${_target} SHARED ${srcFiles})
install(TARGETS ${_target} DESTINATION sdk/lib)
install(FILES ${sdkIncludes} DESTINATION sdk/includes/${_target})

include_directories(${Boost_INCLUDE_DIRS} ../Diameter_Events ../DiameterDictionary ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${_target} DiameterEvents DiameterDictionary)

set_property(TARGET ${_target} PROPERTY VERSION 1.0.0.0)
set_property(TARGET ${_target} PROPERTY SOVERSION 1.0)
set_property(TARGET ${_target} PROPERTY CXX_STANDARD 14)

