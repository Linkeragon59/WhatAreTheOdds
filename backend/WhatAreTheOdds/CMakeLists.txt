cmake_minimum_required (VERSION 3.1.0)

add_library(WhatAreTheOdds)
target_sources(WhatAreTheOdds
	PRIVATE
		Utils.h
		Utils.cpp
		MillenniumFalcon.h
		MillenniumFalcon.cpp
		Empire.h
		Empire.cpp
		OddsCalculator.h
		OddsCalculator.cpp
)

target_compile_features(WhatAreTheOdds PRIVATE cxx_std_23)

target_include_directories(WhatAreTheOdds PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(WhatAreTheOdds PUBLIC nlohmannjson)
target_link_libraries(WhatAreTheOdds PRIVATE sqlite)
