#include <iostream>
#include <cassert>

#include "WhatAreTheOdds.h"

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << " <path/to/millennium-falcon.json> <path/to/empire.json>" << std::endl;
		return EXIT_FAILURE;
	}

	WhatAreTheOdds::MillenniumFalconData millenniumFalconData;
	if (!millenniumFalconData.Parse(argv[1]))
	{
		std::cerr << "Couldn't parse Millennium Falcon data from path: " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}

	WhatAreTheOdds::EmpireData empireData;
	if (!empireData.Parse(argv[2]))
	{
		std::cerr << "Couldn't parse Empire data from path: " << argv[2] << std::endl;
		return EXIT_FAILURE;
	}

	float successOdds = WhatAreTheOdds::Calculator::CalculateSuccessOdds(millenniumFalconData, empireData);
	assert(successOdds >= 0.f && successOdds <= 1.f);
	std::cout << (int)std::round(successOdds * 100.f) << std::endl;

	return EXIT_SUCCESS;
}
