#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

#include "json.hpp"

namespace WhatAreTheOdds
{
	struct MillenniumFalconState
	{
		std::string myLocation;
		int myRemainingFuel = 0;
		int myRemainingDays = 0;
		int myNbEncounters = 0;
	};
	using MillenniumFalconPath = std::vector<MillenniumFalconState>;

	struct MillenniumFalconData
	{
		bool Parse(const char* aJsonPath);

		int myAutonomy = 0;
		std::string myDeparture;
		std::string myArrival;
		std::map<std::string, std::vector<std::pair<std::string, int>>> myRoutes;
	};

	struct EmpireData
	{
		bool Parse(const char* aJsonPath);
		bool Parse(const nlohmann::json& someData);

		int myCountDown = 0;
		std::map<std::string, std::set<int>> myBountyHunters;
	};

	class Calculator
	{
	public:
		Calculator(const MillenniumFalconData& someMillenniumFalconData, const EmpireData& someEmpireData)
			: myMillenniumFalconData(someMillenniumFalconData)
			, myEmpireData(someEmpireData)
		{}

		float CalculateSuccessOdds();

	private:
		std::vector<MillenniumFalconPath> FindPaths(const MillenniumFalconState& aStartState);

		const MillenniumFalconData& myMillenniumFalconData;
		const EmpireData& myEmpireData;
		int myMinNumberOfEncounters = INT_MAX;
	};
}
