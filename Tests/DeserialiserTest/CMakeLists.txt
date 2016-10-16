set(_target DeserialiserTest)

set(srcFiles DiameterTest.cpp)

add_executable(${_target} ${srcFiles})
install(TARGETS ${_target} DESTINATION bin)
install(FILES run.sh DESTINATION bin PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)

set(DIAMETER_DICTIONARIES_ROOT /home/alanmc/Diameter_WS_tar/DiameterDictionary.build/_dist)
find_diameter_dictionary(Base)
find_diameter_dictionary(Gx970)

include_directories(${Boost_INCLUDE_DIRS} ${Base_INCLUDES} ${Gx970_INCLUDES} ${DIAMETER_SDK_INCLUDE_DIRS})

target_link_libraries(${_target} ${DIAMETER_SDK_LIBRARIES} ${Gx970_LIBRARIES} ${Base_LIBRARIES})

set_property(TARGET ${_target} PROPERTY CXX_STANDARD 14)

