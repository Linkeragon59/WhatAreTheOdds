#pragma once

#include <string>
#include <vector>

#include "json.hpp"

namespace WhatAreTheOdds
{
	using Planet = std::string;

	struct Route
	{
		Planet myOrigin;
		Planet myDestination;
		int myTravelTime = 0;
	};

	struct BountyHunter
	{
		Planet myPlanet;
		int myDay = 0;
	};

	struct MillenniumFalconState
	{
		Planet myLocation;
		int myRemainingFuel = 0;
		int myRemainingDays = 0;
	};
	using MillenniumFalconPath = std::vector<MillenniumFalconState>;

	struct MillenniumFalconData
	{
		void Clear();
		bool Parse(const char* aJsonPath);

		int myAutonomy = 0;
		Planet myDeparture;
		Planet myArrival;
		std::vector<Route> myRoutes;
	};

	struct EmpireData
	{
		void Clear();
		bool Parse(const char* aJsonPath);
		bool Parse(const nlohmann::json& someData);

		int myCountDown = 0;
		std::vector<BountyHunter> myBountyHunters;
	};

	class Calculator
	{
	public:
		static float CalculateSuccessOdds(const MillenniumFalconData& someMillenniumFalconData, const EmpireData& someEmpireData);

	private:
		static std::vector<MillenniumFalconPath> FindPaths(const MillenniumFalconState& aStartState, const MillenniumFalconData& someMillenniumFalconData, const EmpireData& someEmpireData);
	};
}
