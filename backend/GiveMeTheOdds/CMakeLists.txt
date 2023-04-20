cmake_minimum_required (VERSION 3.1.0)

add_executable(GiveMeTheOdds)
target_sources(GiveMeTheOdds
	PRIVATE
		main.cpp
)

target_compile_features(GiveMeTheOdds PRIVATE cxx_std_23)

target_link_libraries(GiveMeTheOdds PRIVATE WhatAreTheOdds)
