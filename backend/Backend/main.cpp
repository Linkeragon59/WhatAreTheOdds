#include <memory>
#include <iostream>

#include "json.hpp"
#include <restbed>

#include "OddsCalculator.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <path/to/millennium-falcon.json>" << std::endl;
		return EXIT_FAILURE;
	}

	WhatAreTheOdds::MillenniumFalconData millenniumFalconData;
	if (!millenniumFalconData.Parse(argv[1]))
	{
		std::cerr << "Couldn't parse Millennium Falcon data from path: " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		std::shared_ptr<restbed::Resource> resource = std::make_shared<restbed::Resource>();
		resource->set_path("/odds");
		resource->set_method_handler("OPTIONS", [](const std::shared_ptr<restbed::Session> aSession)
		{
			aSession->close(restbed::OK);
		});
		resource->set_method_handler("POST", [&millenniumFalconData](const std::shared_ptr<restbed::Session> aSession)
		{
			std::shared_ptr<const restbed::Request> request = aSession->get_request();
			size_t contentLength = request->get_header("Content-Length", 0);
			aSession->fetch(contentLength, [request, &millenniumFalconData](const std::shared_ptr<restbed::Session> aSession, const restbed::Bytes& aBody)
			{
				try
				{
					WhatAreTheOdds::EmpireData empireData;
					if (empireData.Parse(nlohmann::json::parse(aBody.begin(), aBody.end())))
					{
						WhatAreTheOdds::Calculator calculator(millenniumFalconData, empireData);
						float successOdds = calculator.CalculateSuccessOdds();

						nlohmann::json jsonResult = {
							{ "odds", std::to_string(successOdds) }
						};
						std::string response = jsonResult.dump();
						aSession->close(restbed::OK, response, { { "Content-Length", std::to_string(response.size()) } });
					}
					else
					{
						std::string response = "The uploaded json file doesn't contain valid Empire data.";
						aSession->close(restbed::BAD_REQUEST, response, { { "Content-Length", std::to_string(response.size()) } });
					}
				}
				catch (const nlohmann::json::parse_error&)
				{
					std::string response = "The uploaded json file couldn't be parsed.";
					aSession->close(restbed::BAD_REQUEST, response, { { "Content-Length", std::to_string(response.size()) } });
				}
			});
		});

		std::shared_ptr<restbed::Settings> settings = std::make_shared<restbed::Settings>();
		settings->set_port(3001);
		settings->set_default_header("Connection", "close");
		settings->set_default_header("Access-Control-Allow-Origin", "*");
		settings->set_default_header("Access-Control-Allow-Headers", "Content-Type");

		std::shared_ptr<restbed::Service> service = std::make_shared<restbed::Service>();
		service->publish(resource);
		service->start(settings);
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << "restbed error: " << error.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::runtime_error& error)
	{
		std::cerr << "restbed error: " << error.what() << std::endl;
		return EXIT_FAILURE;
	}
    
    return EXIT_SUCCESS;
}