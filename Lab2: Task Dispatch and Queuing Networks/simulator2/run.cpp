#include <iostream>
#include <cassert>
#include "json/json.h"
#include "arrival_process.h"
#include "distribution.h"
#include "factory.h"
#include "object_pool.h"
#include "perf_model.h"
#include "random_helper.h"
#include "request.h"
#include "request_generator.h"
#include "request_handler.h"
#include "request_stream.h"
#include "simulator.h"
#include "stats.h"

Json::Value runSimulation(Json::Value jsonRoot, bool verbose)
{
    // Seed random number generator
    if (jsonRoot.isMember("seed")) {
        seedGenerator(jsonRoot["seed"].asUInt());
    } else {
        seedGeneratorRand();
    }

    // Create distributions
    if (jsonRoot.isMember("distributions")) {
        for (auto &itr: jsonRoot["distributions"]) {
            Factory<Distribution>::create(itr);
        }
    }

    // Create performance models
    if (jsonRoot.isMember("perfModels")) {
        for (auto &itr: jsonRoot["perfModels"]) {
            Factory<PerfModel>::create(itr);
        }
    }

    // Create stats
    if (verbose) {
        std::cout << "creating statistics" << std::endl;
    }
    if (jsonRoot.isMember("stats")) {
        for (auto &itr: jsonRoot["stats"]) {
            Factory<Stats>::create(itr);
        }
    }

    // Create request handlers
    if (verbose) {
        std::cout << "creating requestHandlers" << std::endl;
    }
    if (jsonRoot.isMember("requestHandlers")) {
        for (auto &it: jsonRoot["requestHandlers"]) {
            Factory<RequestHandler>::create(it);
        }
    }

    // Create request streams
    if (verbose) {
        std::cout << "creating requestStreams" << std::endl;
    }
    assert(jsonRoot.isMember("requestStreams"));
    for (auto &it: jsonRoot["requestStreams"]) {
        Factory<RequestStream>::create(it);
    }

    // Init stats
    for (auto &it : Factory<Stats>::getObjects()) {
        it.second->init();
    }

    // Init request handlers
    for (auto &it : Factory<RequestHandler>::getObjects()) {
        it.second->init();
    }

    // Init request streams
    for (auto &it : Factory<RequestStream>::getObjects()) {
        it.second->init();
    }

    // Execute simulation
    if (verbose) {
        std::cout << "starting simulation" << std::endl;
    }
    if (jsonRoot.isMember("stopTime")) {
        uint64_t stopTime = jsonRoot["stopTime"].asUInt64();
        simulator::run(stopTime);
    } else {
        simulator::run();
    }
    if (verbose) {
        std::cout << "finished simulation" << std::endl;
    }

    // Gather stats
    for (auto &it : Factory<Stats>::getObjects()) {
        it.second->checkOutputStats(true); // force output on last remaining data
        jsonRoot["results"][it.first] = it.second->getStats();
    }

    // Call clean-up procedures
    Factory<RequestHandler>::finish();
    Factory<RequestStream>::finish();
    Factory<Stats>::finish();
    Factory<ArrivalProcess>::finish();
    Factory<RequestGenerator>::finish();
    Factory<PerfModel>::finish();
    Factory<Distribution>::finish();
    ObjectPool<Request>::finish();
    simulator::reset();

    return jsonRoot;
}
