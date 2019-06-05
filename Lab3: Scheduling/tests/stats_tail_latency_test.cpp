#include <string>
#include "catch/catch.hpp"
#include "json/json.h"
#include "../stats_tail_latency.h"
#include "../simulator.h"
#include "../time_helper.h"
#include "test_helper.h"

TEST_CASE("reset tail latency stats", "[StatsTailLatency]") {
    Json::Value config;
    StatsTailLatency stats(config);

    simulator::setSimTime(convertSecondsToTime(2.0));

    Request* req = Request::create(0, 0);
    REQUIRE(req->getElapsedTime() == convertSecondsToTime(2.0));
    stats.notifyEnd(req);
    REQUIRE(stats.getTailLatency(0) == 2.0);
    REQUIRE(stats.getTailLatency(0.5) == 2.0);
    REQUIRE(stats.getTailLatency(1) == 2.0);
    req->init(convertSecondsToTime(1.0), 0);
    stats.notifyEnd(req);
    REQUIRE(stats.getTailLatency(0) == 1.0);
    REQUIRE(stats.getTailLatency(0.5) == 2.0);
    REQUIRE(stats.getTailLatency(1) == 2.0);
    req->init(convertSecondsToTime(0.5), 0);
    stats.notifyEnd(req);
    REQUIRE(stats.getTailLatency(0) == 1.0);
    REQUIRE(stats.getTailLatency(0.5) == 1.5);
    REQUIRE(stats.getTailLatency(1) == 2.0);

    stats.resetStats();
    req->init(0, 0);
    stats.notifyEnd(req);
    REQUIRE(stats.getTailLatency(0) == 2.0);
    REQUIRE(stats.getTailLatency(0.5) == 2.0);
    REQUIRE(stats.getTailLatency(1) == 2.0);
    req->init(convertSecondsToTime(1.0), 0);
    stats.notifyEnd(req);
    REQUIRE(stats.getTailLatency(0) == 1.0);
    REQUIRE(stats.getTailLatency(0.5) == 2.0);
    REQUIRE(stats.getTailLatency(1) == 2.0);
    req->init(convertSecondsToTime(0.5), 0);
    stats.notifyEnd(req);
    REQUIRE(stats.getTailLatency(0) == 1.0);
    REQUIRE(stats.getTailLatency(0.5) == 1.5);
    REQUIRE(stats.getTailLatency(1) == 2.0);

    simulator::setSimTime(0);

    req->destroy();

    ObjectPool<Request>::finish();
}

TEST_CASE("test for empty tail latency calculation", "[StatsTailLatency]") {
    Json::Value config;
    StatsTailLatency stats(config);

    REQUIRE(stats.getTailLatency(0) == 0);
    REQUIRE(stats.getTailLatency(1.0) == 0);

    stats.resetStats();
    REQUIRE(stats.getTailLatency(0) == 0);
    REQUIRE(stats.getTailLatency(1.0) == 0);

    simulator::setSimTime(convertSecondsToTime(2.0));
    Request* req = Request::create(0, 0);
    stats.notifyEnd(req);
    stats.resetStats();
    REQUIRE(stats.getTailLatency(0) == 0);
    REQUIRE(stats.getTailLatency(1.0) == 0);

    simulator::setSimTime(0);

    req->destroy();

    ObjectPool<Request>::finish();
}

TEST_CASE("test notifyEnd outputs and resets tail latency stats as needed", "[StatsTailLatency]") {
    Json::Value config;
    config["outputJson"] = true;
    config["outputIntervalNs"] = convertSecondsToTime(1.0);
    config["percentiles"][0u] = 0.0;
    config["percentiles"][1] = 0.5;
    config["percentiles"][2] = 1.0;
    StatsTailLatency stats(config);

    simulator::setSimTime(convertSecondsToTime(0.8));
    Request* req = Request::create(0, 0);
    stats.notifyEnd(req);
    REQUIRE(stats.getStats() == Json::nullValue);

    simulator::setSimTime(convertSecondsToTime(1.6));
    req->init(convertSecondsToTime(1.2), 0);
    stats.notifyEnd(req);
    Json::Value jsonStats = stats.getStats();
    REQUIRE(jsonStats.size() == 1);
    REQUIRE(jsonStats[0]["t"].asUInt64() == 0);
    REQUIRE(jsonStats[0][std::to_string(0.0)].asDouble() == 0.8);
    REQUIRE(jsonStats[0][std::to_string(0.5)].asDouble() == 0.8);
    REQUIRE(jsonStats[0][std::to_string(1.0)].asDouble() == 0.8);

    simulator::setSimTime(convertSecondsToTime(1.8));
    req->init(convertSecondsToTime(1.6), 0);
    stats.notifyEnd(req);
    jsonStats = stats.getStats();
    REQUIRE(jsonStats.size() == 1);
    REQUIRE(jsonStats[0]["t"].asUInt64() == 0);
    REQUIRE(jsonStats[0][std::to_string(0.0)].asDouble() == 0.8);
    REQUIRE(jsonStats[0][std::to_string(0.5)].asDouble() == 0.8);
    REQUIRE(jsonStats[0][std::to_string(1.0)].asDouble() == 0.8);

    simulator::setSimTime(convertSecondsToTime(2.0));
    req->init(convertSecondsToTime(1.8), 0);
    stats.notifyEnd(req);
    jsonStats = stats.getStats();
    REQUIRE(jsonStats.size() == 2);
    REQUIRE(jsonStats[0]["t"].asUInt64() == 0);
    REQUIRE(jsonStats[0][std::to_string(0.0)].asDouble() == 0.8);
    REQUIRE(jsonStats[0][std::to_string(0.5)].asDouble() == 0.8);
    REQUIRE(jsonStats[0][std::to_string(1.0)].asDouble() == 0.8);
    REQUIRE(jsonStats[1]["t"].asUInt64() == convertSecondsToTime(1.0));
    REQUIRE(jsonStats[1][std::to_string(0.0)].asDouble() == 0.2);
    REQUIRE(jsonStats[1][std::to_string(0.5)].asDouble() == 0.4);
    REQUIRE(jsonStats[1][std::to_string(1.0)].asDouble() == 0.4);

    simulator::setSimTime(0);

    req->destroy();

    ObjectPool<Request>::finish();
}
