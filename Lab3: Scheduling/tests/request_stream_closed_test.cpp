#include "catch/catch.hpp"
#include "json/json.h"
#include "../request_stream_closed.h"
#include "../request_handler.h"
#include "../request_generator.h"
#include "../distribution.h"
#include "../request.h"
#include "../simulator.h"
#include "../time_helper.h"
#include "test_request_handler.h"

TEST_CASE("test closed loop request stream", "[RequestStreamClosed]") {
    simulator::reset();
    TestRequestHandler* reqHandler = createTestRequestHandler("testRequestHandler", 100);
    Json::Value config;
    int MPL;
    double thinkTime;
    SECTION("test MPL 1 think time 0") {
        MPL = 1;
        thinkTime = 0;
    }
    SECTION("test MPL 1 think time 2") {
        MPL = 1;
        thinkTime = 2.0;
    }
    SECTION("test MPL 3 think time 0") {
        MPL = 3;
        thinkTime = 0;
    }
    SECTION("test MPL 3 think time 2") {
        MPL = 3;
        thinkTime = 2.0;
    }
    config["MPL"] = MPL;
    config["thinkTime"]["name"] = "thinkTime_dist";
    config["thinkTime"]["type"] = "DistributionDet";
    config["thinkTime"]["val"] = thinkTime;
    config["requestGeneratorConfig"]["name"] = "testRequestGenerator";
    config["requestGeneratorConfig"]["type"] = "RequestGeneratorDist";
    config["requestGeneratorConfig"]["dist"]["name"] = "1000bytes_dist";
    config["requestGeneratorConfig"]["dist"]["type"] = "DistributionDet";
    config["requestGeneratorConfig"]["dist"]["val"] = 1000.0;
    config["requestHandler"] = "testRequestHandler";
    RequestStreamClosed reqStream(config);
    reqStream.init();

    simulator::run(convertSecondsToTime(1.0));
    if (thinkTime < 1.0) {
        REQUIRE(reqHandler->numArrivals() == MPL);
        REQUIRE(reqHandler->getQueueLength() == MPL);
        REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(MPL * 9));
    } else {
        REQUIRE(reqHandler->numArrivals() == 0);
        REQUIRE(reqHandler->getQueueLength() == 0);
        REQUIRE(reqHandler->getRemainingWorkLeft() == 0);
    }
    REQUIRE(reqHandler->numCompletions() == 0);

    simulator::run(convertSecondsToTime(thinkTime + 1.0));
    REQUIRE(reqHandler->numArrivals() == MPL);
    REQUIRE(reqHandler->numCompletions() == 0);
    REQUIRE(reqHandler->getQueueLength() == MPL);
    REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(MPL * 9));

    simulator::run(convertSecondsToTime(thinkTime + 11.0));
    if (thinkTime < 1.0) {
        REQUIRE(reqHandler->numArrivals() == 2 * MPL);
        REQUIRE(reqHandler->getQueueLength() == MPL);
        REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(MPL * 9));
    } else {
        REQUIRE(reqHandler->numArrivals() == MPL);
        REQUIRE(reqHandler->getQueueLength() == 0);
        REQUIRE(reqHandler->getRemainingWorkLeft() == 0);
    }
    REQUIRE(reqHandler->numCompletions() == MPL);

    simulator::run(convertSecondsToTime(2.0 * thinkTime + 11.0));
    REQUIRE(reqHandler->numArrivals() == 2 * MPL);
    REQUIRE(reqHandler->numCompletions() == MPL);
    REQUIRE(reqHandler->getQueueLength() == MPL);
    REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(MPL * 9));

    Factory<RequestHandler>::finish();
    Factory<RequestGenerator>::finish();
    Factory<PerfModel>::finish();
    Factory<Distribution>::finish();
    ObjectPool<Request>::finish();
    simulator::reset();
}
