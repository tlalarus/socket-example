cmake_minimum_required(VERSION 3.19)
set(BIN_NAME "${PROJECT_NAME}-adapter")

include_directories(${Boost_INCLUDE_DIRS})
add_executable(${BIN_NAME}
        main.cpp
        adapter.cpp
        model.cpp
        model_api.cpp)

target_link_libraries(${BIN_NAME} ${Boost_LIBRARIES})