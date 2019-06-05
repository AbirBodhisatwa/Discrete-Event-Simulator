#include "catch/catch.hpp"
#include "json/json.h"
#include "../stats_latency.h"
#include "../simulator.h"
#include "../time_helper.h"
#include "test_helper.h"

TEST_CASE("reset latency stats", "[StatsLatency]") {
    Json::Value config;
    StatsLatency stats(config);

    simulator::setSimTime(convertSecondsToTime(2.0));

    Request* req = Request::create(0, 0);
    REQUIRE(req->getElapsedTime() == convertSecondsToTime(2.0));
    stats.notifyEnd(req);
    REQUIRE(stats.getMeanLatency() == 2.0);
    req->init(convertSecondsToTime(1.0), 0);
    stats.notifyEnd(req);
    REQUIRE(stats.getMeanLatency() == 1.5);

    stats.resetStats();
    req->init(0, 0);
    stats.notifyEnd(req);
    REQUIRE(stats.getMeanLatency() == 2.0);
    req->init(convertSecondsToTime(1.0), 0);
    stats.notifyEnd(req);
    REQUIRE(stats.getMeanLatency() == 1.5);

    simulator::setSimTime(0);

    req->destroy();

    ObjectPool<Request>::finish();
}

TEST_CASE("test for divide by 0 in latency calculation", "[StatsLatency]") {
    Json::Value config;
    StatsLatency stats(config);

    REQUIRE(stats.getMeanLatency() == 0);

    stats.resetStats();
    REQUIRE(stats.getMeanLatency() == 0);

    simulator::setSimTime(convertSecondsToTime(2.0));
    Request* req = Request::create(0, 0);
    stats.notifyEnd(req);
    stats.resetStats();
    REQUIRE(stats.getMeanLatency() == 0);

    simulator::setSimTime(0);

    req->destroy();

    ObjectPool<Request>::finish();
}

TEST_CASE("test notifyEnd outputs and resets latency stats as needed", "[StatsLatency]") {
    Json::Value config;
    config["outputJson"] = true;
    config["outputIntervalNs"] = convertSecondsToTime(1.0);
    StatsLatency stats(config);

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
    REQUIRE(jsonStats[0]["E[T]"].asDouble() == 0.8);

    simulator::setSimTime(convertSecondsToTime(1.8));
    req->init(convertSecondsToTime(1.6), 0);
    stats.notifyEnd(req);
    jsonStats = stats.getStats();
    REQUIRE(jsonStats.size() == 1);
    REQUIRE(jsonStats[0]["t"].asUInt64() == 0);
    REQUIRE(jsonStats[0]["E[T]"].asDouble() == 0.8);

    simulator::setSimTime(convertSecondsToTime(2.0));
    req->init(convertSecondsToTime(1.8), 0);
    stats.notifyEnd(req);
    jsonStats = stats.getStats();
    REQUIRE(jsonStats.size() == 2);
    REQUIRE(jsonStats[0]["t"].asUInt64() == 0);
    REQUIRE(jsonStats[0]["E[T]"].asDouble() == 0.8);
    REQUIRE(jsonStats[1]["t"].asUInt64() == convertSecondsToTime(1.0));
    REQUIRE(jsonStats[1]["E[T]"].asDouble() == 0.3);

    simulator::setSimTime(0);

    req->destroy();

    ObjectPool<Request>::finish();
}
