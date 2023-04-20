cmake_minimum_required (VERSION 3.1.0)

add_library(nlohmannjson INTERFACE)

target_include_directories(nlohmannjson INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/json/single_include/nlohmann)
