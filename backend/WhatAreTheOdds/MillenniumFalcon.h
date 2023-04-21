#pragma once

#include <string>
#include <vector>
#include <map>

namespace WhatAreTheOdds
{
	struct MillenniumFalconData
	{
		bool Parse(const char* aJsonPath);

		int myAutonomy = 0;
		std::string myDeparture;
		std::string myArrival;
		std::map<std::string, std::vector<std::pair<std::string, int>>> myRoutes;
	};
}
