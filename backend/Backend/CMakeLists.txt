cmake_minimum_required (VERSION 3.1.0)

add_executable(Backend)
target_sources(Backend
	PRIVATE
		main.cpp
)

target_compile_features(Backend PRIVATE cxx_std_23)

target_include_directories(Backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Backend PRIVATE restbed)
target_link_libraries(Backend PRIVATE nlohmannjson)
target_link_libraries(Backend PRIVATE WhatAreTheOdds)
