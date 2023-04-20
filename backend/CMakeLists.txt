cmake_minimum_required (VERSION 3.1.0)

project(WhatAreTheOdds)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(Dependencies/nlohmannjson)
set_target_properties(nlohmannjson PROPERTIES FOLDER "Dependencies")
add_subdirectory(Dependencies/sqlite)
set_target_properties(sqlite PROPERTIES FOLDER "Dependencies")
add_subdirectory(Dependencies/asio)
set_target_properties(asio PROPERTIES FOLDER "Dependencies")
add_subdirectory(Dependencies/restbed)
set_target_properties(restbed PROPERTIES FOLDER "Dependencies")

add_subdirectory(WhatAreTheOdds)
add_subdirectory(GiveMeTheOdds)
add_subdirectory(Backend)
