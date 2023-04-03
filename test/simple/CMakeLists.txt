cmake_minimum_required(VERSION 3.19)

# FetchContent 모듈 추가
include(FetchContent)

message(STATUS "Fetching googletest...")
# 외부 프로젝트 저장소 정의
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

project(test-simple)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

# googletest의 프로퍼티를 가져와서 현재 프로젝트가 populate 됬는지 치크
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    # populate 되지 않았으면 FetchContent_Populate를 통해서
    # googletest를 빌드하기 위한 sub-build 구성

    # <name>_SOURCE_DIR: ./build/_deps/<name>-src (외부 프로젝트의 저장소에 있는 전체파일)
    # <name>_BINARY_DIR: ./build/_deps/<name>-build (외부 프로젝트를 빌드해서 얻은 라이브러리 )
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    include_directories(${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)
endif()