set(_target DiameterGenerator)

set(srcFiles Application.cpp Avp.cpp Command.cpp 
	Config.cpp GroupOfAvps.cpp main.cpp impl/MessageGenerator.cpp
	impl/TemplateApplicator.cpp impl/RapidDictionaryParser.cpp)

file(GLOB templateFiles templates/*.tmpl)

add_executable(${_target} ${srcFiles})
include_directories(${Boost_INCLUDE_DIRS} ${Tclap_INCLUDE_DIRS} ${RapidXML_INCLUDE_DIRS})

install(TARGETS ${_target} DESTINATION sdk/bin)
install(FILES ${templateFiles} DESTINATION sdk/bin)

set_property(TARGET ${_target} PROPERTY CXX_STANDARD 14)

