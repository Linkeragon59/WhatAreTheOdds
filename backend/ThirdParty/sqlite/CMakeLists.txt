cmake_minimum_required(VERSION 3.1.0)

add_library(sqlite)
target_sources(sqlite
	PRIVATE
		3410200/sqlite3.h
		3410200/sqlite3.c
)

target_compile_features(sqlite PRIVATE cxx_std_23)

target_include_directories(sqlite PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/3410200)
