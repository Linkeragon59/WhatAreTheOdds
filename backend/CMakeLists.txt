cmake_minimum_required (VERSION 3.1.0)

project(WhatAreTheOdds)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(ThirdParty/nlohmannjson)
set_target_properties(nlohmannjson PROPERTIES FOLDER "Dependencies")
add_subdirectory(ThirdParty/sqlite)
set_target_properties(sqlite PROPERTIES FOLDER "Dependencies")
add_subdirectory(ThirdParty/restbed)
set_target_properties(asio PROPERTIES FOLDER "Dependencies")
set_target_properties(restbed PROPERTIES FOLDER "Dependencies")

add_subdirectory(WhatAreTheOdds)
add_subdirectory(GiveMeTheOdds)
add_subdirectory(Backend)
