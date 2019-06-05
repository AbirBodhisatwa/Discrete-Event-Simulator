#include "catch/catch.hpp"
#include "json/json.h"
#include "../stats_throughput.h"
#include "../simulator.h"
#include "../time_helper.h"
#include "test_helper.h"

TEST_CASE("reset throughput stats", "[StatsThroughput]") {
    Json::Value config;
    StatsThroughput stats(config);

    Request* req = Request::create(0, 0);
    stats.notifyEnd(req);
    REQUIRE(stats.getThroughput(convertSecondsToTime(1.0)) == 1.0);
    REQUIRE(stats.getThroughput(convertSecondsToTime(2.0)) == 0.5);
    stats.notifyEnd(req);
    REQUIRE(stats.getThroughput(convertSecondsToTime(1.0)) == 2.0);
    REQUIRE(stats.getThroughput(convertSecondsToTime(2.0)) == 1.0);

    stats.resetStats();
    stats.notifyEnd(req);
    REQUIRE(stats.getThroughput(convertSecondsToTime(1.0)) == 1.0);
    REQUIRE(stats.getThroughput(convertSecondsToTime(2.0)) == 0.5);
    stats.notifyEnd(req);
    REQUIRE(stats.getThroughput(convertSecondsToTime(1.0)) == 2.0);
    REQUIRE(stats.getThroughput(convertSecondsToTime(2.0)) == 1.0);

    req->destroy();

    ObjectPool<Request>::finish();
}

TEST_CASE("test for divide by 0 in throughput calculation", "[StatsThroughput]") {
    Json::Value config;
    StatsThroughput stats(config);

    REQUIRE(stats.getThroughput(0) == 0);

    stats.resetStats();
    REQUIRE(stats.getThroughput(0) == 0);
}

TEST_CASE("test notifyEnd outputs and resets throughput stats as needed", "[StatsThroughput]") {
    Json::Value config;
    config["outputJson"] = true;
    config["outputIntervalNs"] = convertSecondsToTime(1.0);
    StatsThroughput stats(config);

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
    REQUIRE(jsonStats[0]["X"].asDouble() == 1.0);

    simulator::setSimTime(convertSecondsToTime(1.8));
    req->init(convertSecondsToTime(1.6), 0);
    stats.notifyEnd(req);
    jsonStats = stats.getStats();
    REQUIRE(jsonStats.size() == 1);
    REQUIRE(jsonStats[0]["t"].asUInt64() == 0);
    REQUIRE(jsonStats[0]["X"].asDouble() == 1.0);

    simulator::setSimTime(convertSecondsToTime(2.0));
    req->init(convertSecondsToTime(1.8), 0);
    stats.notifyEnd(req);
    jsonStats = stats.getStats();
    REQUIRE(jsonStats.size() == 2);
    REQUIRE(jsonStats[0]["t"].asUInt64() == 0);
    REQUIRE(jsonStats[0]["X"].asDouble() == 1.0);
    REQUIRE(jsonStats[1]["t"].asUInt64() == convertSecondsToTime(1.0));
    REQUIRE(jsonStats[1]["X"].asDouble() == 2.0);

    simulator::setSimTime(0);

    req->destroy();

    ObjectPool<Request>::finish();
}
