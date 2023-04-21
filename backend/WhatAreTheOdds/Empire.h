#pragma once

#include <string>
#include <set>
#include <map>
#include "json.hpp"

namespace WhatAreTheOdds
{
	struct EmpireData
	{
		bool Parse(const char* aJsonPath);
		bool Parse(const nlohmann::json& someData);

		int myCountDown = 0;
		std::map<std::string, std::set<int>> myBountyHunters;
	};
}
