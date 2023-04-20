cmake_minimum_required(VERSION 3.1.0)

add_library(asio INTERFACE)

target_include_directories(asio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/1.18.2/include)
