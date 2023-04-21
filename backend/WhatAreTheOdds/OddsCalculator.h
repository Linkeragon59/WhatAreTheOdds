#pragma once

#include "MillenniumFalcon.h"
#include "Empire.h"

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
