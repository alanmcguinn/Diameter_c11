set(_target BaseDictionary)

diameter_sdk_generate(srcFiles includeFiles includeDir BaseDictionary.xml GENERATE_BASE_AVPS)

add_library(${_target} SHARED ${srcFiles})
install(TARGETS ${_target} DESTINATION ${_target}/lib)
install(FILES ${includeFiles} DESTINATION ${_target}/include)

include_directories(${Boost_INCLUDE_DIRS} ${DIAMETER_SDK_INCLUDE_DIRS})

set_property(TARGET ${_target} PROPERTY VERSION 1.0.0.0)
set_property(TARGET ${_target} PROPERTY SOVERSION 1.0)
set_property(TARGET ${_target} PROPERTY CXX_STANDARD 14)

