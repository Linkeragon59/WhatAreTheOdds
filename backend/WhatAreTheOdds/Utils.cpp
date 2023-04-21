#include "Utils.h"

#include <fstream>

namespace WhatAreTheOdds
{
	bool FileExists(const char* aPath)
	{
		return std::ifstream(aPath).good();
	}

	bool OpenJson(const char* aJsonPath, nlohmann::json& aJsonDataOut)
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
}
