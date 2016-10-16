set(_target DiameterDictionary)

set(srcFiles Command.cpp DictionaryDefinition.cpp GroupedAVP.cpp Message.cpp AVP.cpp)

add_library(${_target} SHARED ${srcFiles})
install(TARGETS ${_target} DESTINATION sdk/lib)
install(FILES AVP.h Command.h CommandEntry.h DictionaryDefinition.h GroupedAVP.h Message.h
            DESTINATION sdk/includes/${_target})

include_directories(${Boost_INCLUDE_DIRS} ../Diameter_Events ../DiameterSerialiser)

target_link_libraries(${_target} DiameterEvents)

set_property(TARGET ${_target} PROPERTY VERSION 1.0.0.0)
set_property(TARGET ${_target} PROPERTY SOVERSION 1.0)
set_property(TARGET ${_target} PROPERTY CXX_STANDARD 14)

