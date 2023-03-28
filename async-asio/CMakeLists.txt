cmake_minimum_required(VERSION 3.19)
set(BIN_NAME "${PROJECT_NAME}-async")

set(BIN_NAME_SERVER "${BIN_NAME}-server")
add_executable(${BIN_NAME_SERVER} async_tcp_echo_server.cpp)

