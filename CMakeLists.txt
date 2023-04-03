cmake_minimum_required(VERSION 3.19)
project(socket)

message("CMAKE_SOURCE: ${CMAKE_SOURCE_DIR}")
message("CMAKE_BINARY: ${CMAKE_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

set(BUILD_TEST ON)
# Cmake configuration options to CMakeCache.txt
option(BUILD_TEST "Build google-test" ON)

# Process subdirectories
#add_library(boost_asio INTERFACE)
#add_library(boost_serialization INTERFACE)
#set_target_properties(boost_serialization PROPERTIES
#        INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
#        INTERFACE_LINK_LIBRARIES "${Boost_SERIALIZATION_LIBRARIES}")

add_subdirectory(src)

if(BUILD_TEST)
    add_subdirectory(test)
else()
    message("pass test")
endif(BUILD_TEST)