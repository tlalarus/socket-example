cmake_minimum_required(VERSION 3.19)

set(BIN_NAME "${PROJECT_NAME}-sync")
set(BIN_NAME_CLIENT "${BIN_NAME}-client")

#find_package(Threads REQUIRED)
#find_package(Boost COMPONENTS filesystem system REQUIRED)

#add_dependencies(${BIN_NAME_SERVER} ${PROJECT_NAME})
#add_executable(${BIN_NAME} server/main.cpp)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(${BIN_NAME_CLIENT} client/main.cpp)
target_link_libraries(${BIN_NAME_CLIENT} ${Boost_LIBRARIES})

