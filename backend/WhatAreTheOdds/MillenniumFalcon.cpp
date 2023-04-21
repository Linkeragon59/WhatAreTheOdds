#include "MillenniumFalcon.h"

#include <filesystem>
#include "sqlite3.h"

#include "Utils.h"

namespace WhatAreTheOdds
{
	namespace MillenniumFalconData_Priv
	{
		int GetDistance(const std::string& aSrc, const std::string& aDst, const std::map<std::string, std::map<std::string, int>>& someRoutes)
		{
			// If the source and the destination are the same planet, we need 0 fuel to stay on the same planet
			if (aSrc == aDst)
				return 0;

			const std::map<std::string, int>& srcNeighbors = someRoutes.at(aSrc);
			auto iter = srcNeighbors.find(aDst);
			if (iter != srcNeighbors.end())
				return iter->second;

			// No direct route between the source and the destination
			return INT_MAX;
		}
	}

	bool MillenniumFalconData::Parse(const char* aJsonPath)
	{
		nlohmann::json data;
		if (!OpenJson(aJsonPath, data))
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
		// First check that the file exists, as sqlite3_open will actually create an empty .db file otherwise
		if (!FileExists(path.string().c_str()) || sqlite3_open(path.string().c_str(), &routesDb) != SQLITE_OK)
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

				int distSrcDest = MillenniumFalconData_Priv::GetDistance(planetDistances[src].first, planetDistances[dst].first, routes);
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
				int distSrcDest = MillenniumFalconData_Priv::GetDistance(planetDistances[src].first, planetDistances[dst].first, routes);
				if (distSrcDest < INT_MAX)
				{
					accessiblePlanets.push_back({ planetDistances[dst].first, distSrcDest });
				}
			}
			myRoutes.insert({ planetDistances[src].first, accessiblePlanets });
		}

		return true;
	}
}
