cmake_minimum_required(VERSION 3.19)
project(socket)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED)

add_library(boost_asio INTERFACE)

set(PROJECT_OUTPUT_DIR ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR})
#set(PROJECT_INCLUDE_DIR ${PROJECT_OUTPUT_DIR}/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)

add_subdirectory(sync)
add_subdirectory(async-asio)