# WhatAreTheOdds

This project is an answer for the following test: https://github.com/lioncowlionant/developer-test

## Run pre-built resources

The build/ folder contains the pre-built resources to let you test the frontend, backend and CLI on Windows or macOS.

### Run the CLI

```sh
> .\build\cli\win64\GiveMeTheOdds.exe examples\example3\millennium-falcon.json examples\example3\empire.json
```

### Run the Backend

```sh
> .\build\backend\win64\Backend.exe examples\example3\millennium-falcon.json
```

### Run the Frontend

You can use npm serve, or python http.server for example.
```sh
> cd build\frontend
> python.exe -m http.server 8080
```
or
```sh
> cd build\frontend
> npm install --global serve
> serve -s
```

## Build the resources

### CLI and Backend

The CLI and the Backend are built together, within the same CMake project. They are implemented with C++.
When cloning this repo, make sure to clone as recursive as it contains dependencies to other repo for JSON and restbed implementations:
- backend/ThirdParty/nlohmannjson : https://github.com/nlohmann/json
- backend/ThirdParty/restbed : https://github.com/Linkeragon59/restbed (forked from https://github.com/Corvusoft/restbed and slightly modified to remove unused dependencies)
- backend/ThirdParty/sqlite : not a git submodule, source has been downloaded from the official website https://www.sqlite.org/download.html

The project can be generated and built with CMake, or generated by CMake and built with Visual Studio or Xcode for example.
```sh
> cmake -Sbackend -Bbackend\build
> cmake --build backend\build --config Release --target all
```

### Frontend

The Frontend has been made using React, after installing Node.js and npm (https://nodejs.org/en/download)
```sh
> npm install -g create-react-app
```

Because the React app is already created, continuing to develop the frontend after pulling from git only requires to install "react-scripts"
```sh
> cd frontend
> npm install react-scripts
> npm start
```

## Implementation details

### Backend and CLI

The Backend and the CLI are using the same library written in C++ : backend/WhatAreTheOdds/
After parsing the Millennium-Falcon JSON (read from the disk) and the Empire JSON data (received from the Frontend), the odds calculation is done as follows:

- The path finding algorithm start at the departure planet (Tatooine). At each stage, it iterates through the accessible locations from the current planet (including the planet itself for waiting or refueling). For each accessible location it does a recursive call the path finding algorithm and adds the current stage to the path. The recursion ends when a path reaches the arrival planet or when the path didn't reach the arrival planet after the Empire count down.

A naive implementation of this algorithm is very inefficient, as it will always iterate all the possible paths given the available routes bewteen the planets, which grows exponentially with the length of the path. So for example if the Empire count down gets longer, even though it's likely that the success odds will increase, the naive algorithm takes much more time to calculate the odds. This is why the following optimizations have been implemented:

- Optimization 1 : The Millennium-Falcon routes data is prepared so that for each planet we can iterate the accessible locations sorted by their shortest distance to the arrival planet (Endor). This sorting is done to ensure that we will first look at the promising paths when iterating over all the possible paths to reach the arrival planet.
- Optimization 2 : The iteration is done in two passes. The first pass ignore the destinations that would lead to an encounter with the bounty hunters, then the second pass only treats those. This helps iterating first through the paths that leads to the less encounters.
- Optimization 3 : The algorithm keeps track of the minimum number of encounters with the bounty hunters for all valid paths that have been found so far, and discard all the paths that already have a greater or equal number of encounters. If we find a path were success odds are 100%, we are guaranteed to stop the iteration immediately, otherwise we continue to iterate hoping to find paths with higher odds. This means we usually stop the iteration very early thanks to Optimization 1 and 2, as we first iterate promizing paths that are likely to have less encounters (and also be shorter). Note that a path can look promising and end up in a trap where encounters with the bounty hunters are inevitable, so this algorithm doesn't guarantee that we always find the best path first.

One more obvious optimization that could be implemented is to start by numbering planets and use the planet indices instead of there names in the calculation. This would both reduce string comparisons and searches in maps.
