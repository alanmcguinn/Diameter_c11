set(_target Gx970Dictionary)

diameter_sdk_generate(srcFiles includeFiles includeDir Gx970_dictionary.xml)

add_library(${_target} SHARED ${srcFiles})
install(TARGETS ${_target} DESTINATION ${_target}/lib)
install(FILES ${includeFiles} DESTINATION ${_target}/include)

MESSAGE(STATUS "Base Include Dir is ${BASE_INCLUDE_DIR}")
include_directories(${Boost_INCLUDE_DIRS} ${DIAMETER_SDK_INCLUDE_DIRS} ${BASE_INCLUDE_DIR})

set_property(TARGET ${_target} PROPERTY VERSION 1.0.0.0)
set_property(TARGET ${_target} PROPERTY SOVERSION 1.0)
set_property(TARGET ${_target} PROPERTY CXX_STANDARD 14)

