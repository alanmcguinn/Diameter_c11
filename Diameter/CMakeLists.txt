cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(Diameter)

set(Boost_USE_MULTITHREADED ON)
set(BOOST_ROOT /opt/boost_1.62.0)
find_package(Boost 1.62.0 COMPONENTS system)

# TCLAP Templatised Command Line Argument Parser
set (Tclap_INCLUDE_DIRS /opt/tclap-1.2.1/include)
MESSAGE(STATUS "TClap: " ${Tclap_INCLUDE_DIRS})
# RapidXML
set(RapidXML_INCLUDE_DIRS /opt/rapidxml-1.13)
MESSAGE(STATUS "Rapid XML: " ${RapidXML_INCLUDE_DIRS})

set(CMAKE_INSTALL_PREFIX _dist)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -ansi")

add_subdirectory(Diameter_Events)
add_subdirectory(DiameterSerialiser)
add_subdirectory(DiameterDictionary)
add_subdirectory(DiameterGenerator)
add_subdirectory(DiameterNet)

#add_subdirectory(Gx970Dictionary_gen)

#add_subdirectory(DeserialiserTest)
