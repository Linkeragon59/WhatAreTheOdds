#pragma once

#include "json.hpp"

namespace WhatAreTheOdds
{
	bool FileExists(const char* aPath);
	bool OpenJson(const char* aJsonPath, nlohmann::json& aJsonDataOut);
}
