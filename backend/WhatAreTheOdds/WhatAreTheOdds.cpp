#include "WhatAreTheOdds.h"

#include <fstream>
#include <filesystem>

#include "sqlite3.h"

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

	int locGetDistance(const std::string& aSrc, const std::string& aDst, const std::map<std::string, std::map<std::string, int>>& someRoutes)
	{
		if (aSrc == aDst)
			return 0;

		const std::map<std::string, int>& srcNeighbors = someRoutes.at(aSrc);
		auto iter = srcNeighbors.find(aDst);
		if (iter != srcNeighbors.end())
			return iter->second;

		return INT_MAX;
	}

	bool MillenniumFalconData::Parse(const char* aJsonPath)
	{
		nlohmann::json data;
		if (!locOpenJson(aJsonPath, data))
			return false;

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
		// First use ifstream::good() to check that the file exists, as sqlite3_open will actually create an empty .db file otherwise
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

			std::map<std::string, std::map<std::string, int>>* routes = reinterpret_cast<std::map<std::string, std::map<std::string, int>>*>(data);
			for (int i = 0; i < 2; ++i)
			{
				if (routes->find(argv[i]) == routes->end())
				{
					routes->insert({ argv[i], std::map<std::string, int>() });
				}
				routes->at(argv[i]).insert({ argv[1 - i], std::stoi(argv[2]) });
			}
			return 0;
		};

		std::map<std::string, std::map<std::string, int>> routes;
		char* err;
		if (sqlite3_exec(routesDb, "SELECT * FROM 'routes'", selectCallback, &routes, &err) != SQLITE_OK)
		{
			sqlite3_free(err);
			sqlite3_close(routesDb);
			return false;
		}

		sqlite3_close(routesDb);

		// Compute planets shortest distance to the arrival (Shortest Path Tree algorithm)
		std::vector<std::pair<std::string, int>> planetDistances(routes.size());
		std::vector<bool> sptSet(routes.size(), false);
		int index = 0;
		for (const auto& iter : routes)
		{
			planetDistances[index++] = { iter.first, (iter.first == myArrival ? 0 : INT_MAX) };
		}
		for (;;)
		{
			int src = -1, minDist = INT_MAX;
			for (int i = 0; i < (int)planetDistances.size(); ++i)
			{
				if (!sptSet[i] && planetDistances[i].second < minDist)
				{
					src = i;
					minDist = planetDistances[i].second;
				}
			}

			if (src == -1)
				break;

			sptSet[src] = true;

			for (int dst = 0; dst < (int)planetDistances.size(); ++dst)
			{
				if (sptSet[dst])
					continue;

				int distSrcDest = locGetDistance(planetDistances[src].first, planetDistances[dst].first, routes);
				if (distSrcDest < INT_MAX && planetDistances[dst].second > planetDistances[src].second + distSrcDest)
				{
					planetDistances[dst].second = planetDistances[src].second + distSrcDest;
				}
			}
		}
		
		// Sort planets by their shortest distance to the arrival
		std::sort(planetDistances.begin(), planetDistances.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) { return a.second < b.second; });

		// Fill the routes containers
		// From this container, for each planet we can quickly iterate the accessible planets sorted by shortest distance to the arrival
		myRoutes.clear();
		for (int src = 0; src < (int)planetDistances.size(); ++src)
		{
			std::vector<std::pair<std::string, int>> accessiblePlanets;
			for (int dst = 0; dst < (int)planetDistances.size(); ++dst)
			{
				int distSrcDest = locGetDistance(planetDistances[src].first, planetDistances[dst].first, routes);
				if (distSrcDest < INT_MAX)
				{
					accessiblePlanets.push_back({ planetDistances[dst].first, distSrcDest });
				}
			}
			myRoutes.insert({ planetDistances[src].first, accessiblePlanets });
		}

		return true;
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
