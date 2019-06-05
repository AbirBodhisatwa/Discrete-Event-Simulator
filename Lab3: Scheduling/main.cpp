#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "json/json.h"
#include "run.h"

int main(int argc, char* argv[])
{
    // Output help if missing config file
    if (argc != 2) {
        std::cerr << argv[0] << " jsonConfig" << std::endl;
        return 1;
    }

    // Try to open and parse json config file
    std::cout << "parsing json config" << std::endl;
    std::string confFileName = std::string(argv[1]);
    std::ifstream inputFile(confFileName);
    if (!inputFile) {
        std::cerr << "error: failed to read config file " << std::endl;
        return -1;
    }
    Json::Reader jsonReader;
    Json::Value jsonRoot;
    if (!jsonReader.parse(inputFile, jsonRoot)) {
        std::cerr << "error: failed to parse config file " << 
            jsonReader.getFormattedErrorMessages();
        return -1;
    }

    // Run simulation
    jsonRoot = runSimulation(jsonRoot, true);

    // Output stats
    std::ofstream res_file;
    std::cout << "writing results to " << confFileName+"_res.json" << std::endl;
    res_file.open(confFileName+"_res.json");
    res_file << jsonRoot;
    res_file.close();

    return 0;
}
