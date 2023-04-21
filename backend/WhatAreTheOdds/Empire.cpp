#include "Empire.h"

#include "Utils.h"

namespace WhatAreTheOdds
{
	bool EmpireData::Parse(const char* aJsonPath)
	{
		nlohmann::json data;
		if (!OpenJson(aJsonPath, data))
			return false;

		return Parse(data);
	}

	bool EmpireData::Parse(const nlohmann::json& someData)
	{
		// Validate the JSON data:
		bool dataValid = someData.contains("countdown") && someData.at("countdown").is_number_integer();
		dataValid &= someData.contains("bounty_hunters") && someData.at("bounty_hunters").is_array();
		if (!dataValid)
			return false;

		someData.at("countdown").get_to(myCountDown);

		myBountyHunters.clear();
		for (const nlohmann::json& bountyHunterData : someData.at("bounty_hunters"))
		{
			bool bountyHunterDataValid = bountyHunterData.contains("planet") && bountyHunterData.at("planet").is_string();
			bountyHunterDataValid &= bountyHunterData.contains("day") && bountyHunterData.at("day").is_number_integer();
			if (!bountyHunterDataValid)
				return false;

			std::string planet = bountyHunterData.at("planet");
			int day = bountyHunterData.at("day");

			if (myBountyHunters.find(planet) == myBountyHunters.end())
			{
				myBountyHunters.insert({ planet, std::set<int>() });
			}
			myBountyHunters.at(planet).insert(day);
		}

		return true;
	}
}
