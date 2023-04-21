#include "OddsCalculator.h"

namespace WhatAreTheOdds
{
	float Calculator::CalculateSuccessOdds()
	{
		// Consider the case where bounty hunters are on the departure planet on day 0 ?
		int startNbEncounters = 0;
		auto bountyHuntersDays = myEmpireData.myBountyHunters.find(myMillenniumFalconData.myDeparture);
		if (bountyHuntersDays != myEmpireData.myBountyHunters.end() && bountyHuntersDays->second.contains(0))
		{
			startNbEncounters = 1;
		}

		std::vector<MillenniumFalconPath> paths = FindPaths({ myMillenniumFalconData.myDeparture, myMillenniumFalconData.myAutonomy, myEmpireData.myCountDown, startNbEncounters });
		if (paths.size() == 0)
			return 0.f;

		return (float)std::pow(0.9f, myMinNumberOfEncounters);
	}

	std::vector<MillenniumFalconPath> Calculator::FindPaths(const MillenniumFalconState& aStartState)
	{
		std::vector<MillenniumFalconPath> paths;

		// If this path already has a greater or equal number of encounters with bounty hunters than the best path found so far, discard it
		if (aStartState.myNbEncounters >= myMinNumberOfEncounters)
		{
			return paths;
		}

		// If we arrived at destination, end the path
		if (aStartState.myLocation == myMillenniumFalconData.myArrival)
		{
			MillenniumFalconPath path;
			path.push_back(aStartState);
			paths.push_back(path);

			// Record the lowest number of encounters we had so far
			myMinNumberOfEncounters = aStartState.myNbEncounters;

			return paths;
		}

		// If we don't have time left, discard this path
		if (aStartState.myRemainingDays <= 0)
		{
			return paths;
		}

		// Evaluate all accessible planets from this state
		auto iter = myMillenniumFalconData.myRoutes.find(aStartState.myLocation);
		if (iter != myMillenniumFalconData.myRoutes.end())
		{
			const std::vector<std::pair<std::string, int>>& accessibleRoutes = iter->second;

			// First iterate accessible planets that don't have bounty hunters upon arrival, then accessible planets that do
			std::vector<int> planetsWithBountyHunters;

			for (int i = 0; i < (int)accessibleRoutes.size(); ++i)
			{
				// Check we have enough fuel and time before taking a route
				if (accessibleRoutes[i].second > aStartState.myRemainingFuel || accessibleRoutes[i].second > aStartState.myRemainingDays)
					continue;

				// If we stay on the same planet, refills fuel and loose one day
				bool stayOnPlanet = accessibleRoutes[i].first == aStartState.myLocation;
				int nextRemainingFuel = stayOnPlanet ? myMillenniumFalconData.myAutonomy : aStartState.myRemainingFuel - accessibleRoutes[i].second;
				int nextRemainingDays = stayOnPlanet ? aStartState.myRemainingDays - 1 : aStartState.myRemainingDays - accessibleRoutes[i].second;

				// Will we encounter bounty hunters by taking this route?
				int nextDay = myEmpireData.myCountDown - nextRemainingDays;
				auto bountyHuntersDays = myEmpireData.myBountyHunters.find(accessibleRoutes[i].first);
				if (bountyHuntersDays != myEmpireData.myBountyHunters.end() && bountyHuntersDays->second.contains(nextDay))
				{
					// Try this route later
					planetsWithBountyHunters.push_back(i);
					continue;
				}

				for (MillenniumFalconPath& path : FindPaths({ accessibleRoutes[i].first, nextRemainingFuel, nextRemainingDays, aStartState.myNbEncounters }))
				{
					path.push_back(aStartState);
					paths.push_back(path);
				}
			}

			for (int i : planetsWithBountyHunters)
			{
				// If we stay on the same planet, refills fuel and loose one day
				bool stayOnPlanet = accessibleRoutes[i].first == aStartState.myLocation;
				int nextRemainingFuel = stayOnPlanet ? myMillenniumFalconData.myAutonomy : aStartState.myRemainingFuel - accessibleRoutes[i].second;
				int nextRemainingDays = stayOnPlanet ? aStartState.myRemainingDays - 1 : aStartState.myRemainingDays - accessibleRoutes[i].second;

				for (MillenniumFalconPath& path : FindPaths({ accessibleRoutes[i].first, nextRemainingFuel, nextRemainingDays, aStartState.myNbEncounters + 1 }))
				{
					path.push_back(aStartState);
					paths.push_back(path);
				}
			}
		}

		return paths;
	}
}
