cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(DiameterDictionaries)

set(Boost_USE_MULTITHREADED ON)
set(BOOST_ROOT /opt/boost_1.62.0)
find_package(Boost 1.62.0 COMPONENTS system)

set(CMAKE_INSTALL_PREFIX _dist)

set(DIAMETER_SDK_ROOT /home/alanmc/Diameter_WS_tar/Diameter.build/_dist/sdk)
find_package(DiameterSDK)

add_subdirectory(DeserialiserTest)
