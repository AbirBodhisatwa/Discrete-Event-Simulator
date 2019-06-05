#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <sys/resource.h>
#include "catch/catch.hpp"
#include "json/json.h"
#include "../run.h"

uint64_t getMemoryUsage()
{
    rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

TEST_CASE("memory leak test", "[overall]") {
    std::ifstream inputFile;
    SECTION("memory leak test open") {
        inputFile.open("example_open.json");
        std::cout << "Memory leak test - open" << std::endl;
    }
    SECTION("memory leak test closed") {
        inputFile.open("example_closed.json");
        std::cout << "Memory leak test - closed" << std::endl;
    }
    SECTION("memory leak test open network") {
        inputFile.open("example_open_network.json");
        std::cout << "Memory leak test - open network" << std::endl;
    }
    SECTION("memory leak test closed network") {
        inputFile.open("example_closed_network.json");
        std::cout << "Memory leak test - closed network" << std::endl;
    }
    std::cout << "sim_duration\tmemory_usage" << std::endl;
    REQUIRE(inputFile);
    Json::Reader jsonReader;
    Json::Value jsonRoot;
    bool success = jsonReader.parse(inputFile, jsonRoot);
    REQUIRE(success);

    // Disable printing and json results storing
    for (auto & it : jsonRoot["stats"]) {
        if (it.isMember("outputPrint")) {
            it["outputPrint"] = false;
        }
        if (it.isMember("outputJson")) {
            it["outputJson"] = false;
        }
    }
    
    // Run simulation
    uint64_t stopTime = jsonRoot["stopTime"].asUInt64();

    jsonRoot["stopTime"] = stopTime;
    runSimulation(jsonRoot, false);
    uint64_t mem1 = getMemoryUsage();

    jsonRoot["stopTime"] = 2 * stopTime;
    runSimulation(jsonRoot, false);
    uint64_t mem2 = getMemoryUsage();

    jsonRoot["stopTime"] = 4 * stopTime;
    runSimulation(jsonRoot, false);
    uint64_t mem3 = getMemoryUsage();

    jsonRoot["stopTime"] = 8 * stopTime;
    runSimulation(jsonRoot, false);
    uint64_t mem4 = getMemoryUsage();

    jsonRoot["stopTime"] = 16 * stopTime;
    runSimulation(jsonRoot, false);
    uint64_t mem5 = getMemoryUsage();

    jsonRoot["stopTime"] = 32 * stopTime;
    runSimulation(jsonRoot, false);
    uint64_t mem6 = getMemoryUsage();

    jsonRoot["stopTime"] = 64 * stopTime;
    runSimulation(jsonRoot, false);
    uint64_t mem7 = getMemoryUsage();

    std::cout << "1\t\t" << mem1 << std::endl;
    std::cout << "2\t\t" << mem2 << std::endl;
    std::cout << "4\t\t" << mem3 << std::endl;
    std::cout << "8\t\t" << mem4 << std::endl;
    std::cout << "16\t\t" << mem5 << std::endl;
    std::cout << "32\t\t" << mem6 << std::endl;
    std::cout << "64\t\t" << mem7 << std::endl;

    REQUIRE(mem1 == Approx(mem7).epsilon(0.6));
    REQUIRE(mem2 == Approx(mem7).epsilon(0.5));
    REQUIRE(mem3 == Approx(mem7).epsilon(0.4));
    REQUIRE(mem4 == Approx(mem7).epsilon(0.3));
    REQUIRE(mem5 == Approx(mem7).epsilon(0.2));
    REQUIRE(mem6 == Approx(mem7).epsilon(0.1));
}
