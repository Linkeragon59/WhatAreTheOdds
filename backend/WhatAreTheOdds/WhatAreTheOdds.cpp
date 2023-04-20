#include "WhatAreTheOdds.h"

#include <fstream>
#include <filesystem>

#include "sqlite3.h"

#define WHAT_ARE_THE_ODDS_VERBOSE 0
#if WHAT_ARE_THE_ODDS_VERBOSE
#include <iostream>
#endif

namespace WhatAreTheOdds
{
	bool locOpenJson(const char* aJsonPath, nlohmann::json& aJsonDataOut)
	{
		std::ifstream stream(aJsonPath);
		if (!stream.is_open())
			return false;

		try
		{
			aJsonDataOut = nlohmann::json::parse(stream);
			return true;
		}
		catch (const nlohmann::json::parse_error&)
		{
			return false;
		}
	}

	void MillenniumFalconData::Clear()
	{
		myAutonomy = 0;
		myDeparture.clear();
		myArrival.clear();
		myRoutes.clear();
	}

	bool MillenniumFalconData::Parse(const char* aJsonPath)
	{
		nlohmann::json data;
		if (!locOpenJson(aJsonPath, data))
			return false;

		Clear();

		// Validate the JSON data:
		bool dataValid = data.contains("autonomy") && data.at("autonomy").is_number_integer();
		dataValid &= data.contains("departure") && data.at("departure").is_string();
		dataValid &= data.contains("arrival") && data.at("arrival").is_string();
		dataValid &= data.contains("routes_db") && data.at("routes_db").is_string();
		if (!dataValid)
			return false;

		data.at("autonomy").get_to(myAutonomy);
		data.at("departure").get_to(myDeparture);
		data.at("arrival").get_to(myArrival);

		sqlite3* routesDb;
		std::filesystem::path path(aJsonPath);
		path.replace_filename(data.at("routes_db").get<std::string>());
		// First use ifstream::good() to check that the file exists as sqlite3_open will actually create an empty .db file otherwise
		if (!std::ifstream(path.string().c_str()).good() || sqlite3_open(path.string().c_str(), &routesDb) != SQLITE_OK)
			return false;

		auto selectCallback = [](void* data, int argc, char** argv, char** azColName) -> int {
			if (argc != 3)
				return 1;

			const char* columns[3] = { "origin" , "destination" , "travel_time" };
			for (int i = 0; i < 3; ++i)
			{
				if (!argv[i] || !azColName[i] || strcmp(azColName[i], columns[i]) != 0)
					return 1;
			}

			std::vector<Route>* routes = reinterpret_cast<std::vector<Route>*>(data);
			routes->push_back({ argv[0], argv[1], std::stoi(argv[2]) });
			return 0;
		};

		char* err;
		bool success = sqlite3_exec(routesDb, "SELECT * FROM 'routes'", selectCallback, &myRoutes, &err) == SQLITE_OK;
		if (!success)
		{
			sqlite3_free(err);
		}

		sqlite3_close(routesDb);
		return success;
	}

	void EmpireData::Clear()
	{
		myCountDown = 0;
		myBountyHunters.clear();
	}

	bool EmpireData::Parse(const char* aJsonPath)
	{
		nlohmann::json data;
		if (!locOpenJson(aJsonPath, data))
			return false;

		return Parse(data);
	}

	bool EmpireData::Parse(const nlohmann::json& someData)
	{
		Clear();

		// Validate the JSON data:
		bool dataValid = someData.contains("countdown") && someData.at("countdown").is_number_integer();
		dataValid &= someData.contains("bounty_hunters") && someData.at("bounty_hunters").is_array();
		if (!dataValid)
			return false;

		someData.at("countdown").get_to(myCountDown);
		for (const nlohmann::json& bountyHunterData : someData.at("bounty_hunters"))
		{
			bool bountyHunterDataValid = bountyHunterData.contains("planet") && bountyHunterData.at("planet").is_string();
			bountyHunterDataValid &= bountyHunterData.contains("day") && bountyHunterData.at("day").is_number_integer();
			if (!bountyHunterDataValid)
				return false;

			BountyHunter bountyHunter;
			bountyHunterData.at("planet").get_to(bountyHunter.myPlanet);
			bountyHunterData.at("day").get_to(bountyHunter.myDay);
			myBountyHunters.push_back(bountyHunter);
		}

		return true;
	}

	float Calculator::CalculateSuccessOdds(const MillenniumFalconData& someMillenniumFalconData, const EmpireData& someEmpireData)
	{
		float bestSuccessOdds = 0.f;

#if GIVE_ME_THE_ODDS_VERBOSE
		std::cout << "--- Calculate Success Odds ---" << std::endl;
		std::cout << "Departure: " << someMillenniumFalconData.myDeparture << ", Arrival: " << someMillenniumFalconData.myArrival << std::endl;
		std::cout << "Autonomy: " << someMillenniumFalconData.myAutonomy << ", Count Down: " << someEmpireData.myCountDown << std::endl;
		std::cout << "Routes:" << std::endl;
		for (auto route : someMillenniumFalconData.myRoutes)
			std::cout << route.myOrigin << " - " << route.myDestination << " - " << route.myTravelTime << std::endl;
		std::cout << "Bounty Hunters:" << std::endl;
		for (const BountyHunter& bountyHunter : someEmpireData.myBountyHunters)
			std::cout << bountyHunter.myPlanet << " - " << bountyHunter.myDay << std::endl;
#endif

		for (const MillenniumFalconPath& path : FindPaths({ someMillenniumFalconData.myDeparture, someMillenniumFalconData.myAutonomy, someEmpireData.myCountDown }, someMillenniumFalconData, someEmpireData))
		{
#if GIVE_ME_THE_ODDS_VERBOSE
			std::cout << "Possible Path:" << std::endl;
#endif
			int numEncounters = 0;
			for (int i = (int)path.size() - 1; i >= 0; --i)
			{
				const MillenniumFalconState& state = path[i];
				for (const BountyHunter& bountyHunter : someEmpireData.myBountyHunters)
				{
					if (bountyHunter.myPlanet == state.myLocation && bountyHunter.myDay == (someEmpireData.myCountDown - state.myRemainingDays))
					{
						numEncounters++;
						break;
					}
				}
#if GIVE_ME_THE_ODDS_VERBOSE
				std::cout << state.myLocation << " - Remaining Fuel: " << state.myRemainingFuel << " - Remaining Time: " << state.myRemainingDays << std::endl;
#endif
			}

			float pathSuccessOdds = (float)std::pow(0.9f, numEncounters);
#if GIVE_ME_THE_ODDS_VERBOSE
			std::cout << "Path success odds: " << pathSuccessOdds << std::endl;
#endif
			bestSuccessOdds = std::max(bestSuccessOdds, pathSuccessOdds);
		}

#if GIVE_ME_THE_ODDS_VERBOSE
		std::cout << "Best success odds: " << bestSuccessOdds << std::endl;
#endif
		return bestSuccessOdds;
	}

	std::vector<MillenniumFalconPath> Calculator::FindPaths(const MillenniumFalconState& aStartState, const MillenniumFalconData& someMillenniumFalconData, const EmpireData& someEmpireData)
	{
		// If we arrived at destination, end the path
		if (aStartState.myLocation == someMillenniumFalconData.myArrival)
		{
			MillenniumFalconPath path;
			path.push_back(aStartState);
			std::vector<MillenniumFalconPath> paths;
			paths.push_back(path);
			return paths;
		}

		// If we don't have time left, discard this path
		if (aStartState.myRemainingDays <= 0)
		{
			return std::vector<MillenniumFalconPath>();
		}

		std::vector<MillenniumFalconPath> paths;

		// Evaluate all accessible planets from this state
		for (const Route& route : someMillenniumFalconData.myRoutes)
		{
			if (route.myOrigin != aStartState.myLocation && route.myDestination != aStartState.myLocation)
				continue;

			// Check we have enough fuel and time before taking a route
			if (route.myTravelTime > aStartState.myRemainingFuel || route.myTravelTime > aStartState.myRemainingDays)
				continue;

			const std::string& nextDestination = route.myOrigin != aStartState.myLocation ? route.myOrigin : route.myDestination;
			int nextRemainingFuel = aStartState.myRemainingFuel - route.myTravelTime;
			int nextRemainingDays = aStartState.myRemainingDays - route.myTravelTime;
			for (MillenniumFalconPath& path : FindPaths({ nextDestination, nextRemainingFuel, nextRemainingDays }, someMillenniumFalconData, someEmpireData))
			{
				path.push_back(aStartState);
				paths.push_back(path);
			}
		}

		// Last option is to stay on the current planet
		for (MillenniumFalconPath& path : FindPaths({ aStartState.myLocation, someMillenniumFalconData.myAutonomy, aStartState.myRemainingDays - 1 }, someMillenniumFalconData, someEmpireData))
		{
			path.push_back(aStartState);
			paths.push_back(path);
		}

		return paths;
	}
}
