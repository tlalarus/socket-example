cmake_minimum_required(VERSION 3.19)
project(socket)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS filesystem system serialization REQUIRED)

# Common include path
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# 실행 프로그램 타겟을 만들고 타겟에 필요한 파일들 나열
add_executable(adapter-cli
        adapter/main.cpp
        adapter/adapter/adapter.cpp
        adapter/model/model.cpp
        adapter/model/model_api.cpp
        )

add_executable(client client-sync/main.cpp)

# 타겟 빌드에 필요한 모듈을 나열한다.(라이브러리, 외부 프로젝트, 실행 프로그램 ..)
target_link_libraries(adapter-cli ${Boost_LIBRARIES} pthread)
target_link_libraries(client ${Boost_LIBRARIES} pthread)

# 타겟 빌드에 필요한 헤더의 디렉토리를 포함시킨다.
target_include_directories(adapter-cli PUBLIC ${PROJECT_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
target_include_directories(client PUBLIC ${PROJECT_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

message("PROJECT_INCLUDE: ${PROJECT_INCLUDE_DIR}")