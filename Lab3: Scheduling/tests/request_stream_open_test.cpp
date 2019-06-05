#include "catch/catch.hpp"
#include "json/json.h"
#include "../request_stream_open.h"
#include "../request_handler.h"
#include "../request_generator.h"
#include "../distribution.h"
#include "../request.h"
#include "../simulator.h"
#include "../time_helper.h"
#include "test_request_handler.h"

TEST_CASE("test open loop request stream", "[RequestStreamOpen]") {
    simulator::reset();
    TestRequestHandler* reqHandler = createTestRequestHandler("testRequestHandler", 100);
    Json::Value config;
    config["arrivalProcessConfig"]["name"] = "testArrivalProcess";
    config["arrivalProcessConfig"]["type"] = "ArrivalProcessDist";
    config["arrivalProcessConfig"]["dist"]["name"] = "2.0sec_interarrival";
    config["arrivalProcessConfig"]["dist"]["type"] = "DistributionDet";
    config["arrivalProcessConfig"]["dist"]["val"] = 2.0;
    config["requestGeneratorConfig"]["name"] = "testRequestGenerator";
    config["requestGeneratorConfig"]["type"] = "RequestGeneratorDist";
    config["requestGeneratorConfig"]["dist"]["name"] = "1000bytes_dist";
    config["requestGeneratorConfig"]["dist"]["type"] = "DistributionDet";
    config["requestGeneratorConfig"]["dist"]["val"] = 1000.0;
    config["requestHandler"] = "testRequestHandler";
    RequestStreamOpen reqStream(config);
    reqStream.init();

    simulator::run(convertSecondsToTime(1.0));
    REQUIRE(reqHandler->numArrivals() == 0);
    REQUIRE(reqHandler->numCompletions() == 0);
    REQUIRE(reqHandler->getQueueLength() == 0);
    REQUIRE(reqHandler->getRemainingWorkLeft() == 0);

    simulator::run(convertSecondsToTime(3.0));
    REQUIRE(reqHandler->numArrivals() == 1);
    REQUIRE(reqHandler->numCompletions() == 0);
    REQUIRE(reqHandler->getQueueLength() == 1);
    REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(9));

    simulator::run(convertSecondsToTime(5.0));
    REQUIRE(reqHandler->numArrivals() == 2);
    REQUIRE(reqHandler->numCompletions() == 0);
    REQUIRE(reqHandler->getQueueLength() == 2);
    REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(9 + 7));

    simulator::run(convertSecondsToTime(7.0));
    REQUIRE(reqHandler->numArrivals() == 3);
    REQUIRE(reqHandler->numCompletions() == 0);
    REQUIRE(reqHandler->getQueueLength() == 3);
    REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(9 + 7 + 5));

    simulator::run(convertSecondsToTime(9.0));
    REQUIRE(reqHandler->numArrivals() == 4);
    REQUIRE(reqHandler->numCompletions() == 0);
    REQUIRE(reqHandler->getQueueLength() == 4);
    REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(9 + 7 + 5 + 3));

    simulator::run(convertSecondsToTime(11.0));
    REQUIRE(reqHandler->numArrivals() == 5);
    REQUIRE(reqHandler->numCompletions() == 0);
    REQUIRE(reqHandler->getQueueLength() == 5);
    REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(9 + 7 + 5 + 3 + 1));

    simulator::run(convertSecondsToTime(13.0));
    REQUIRE(reqHandler->numArrivals() == 6);
    REQUIRE(reqHandler->numCompletions() == 1);
    REQUIRE(reqHandler->getQueueLength() == 5);
    REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(9 + 7 + 5 + 3 + 1));

    simulator::run(convertSecondsToTime(15.0));
    REQUIRE(reqHandler->numArrivals() == 7);
    REQUIRE(reqHandler->numCompletions() == 2);
    REQUIRE(reqHandler->getQueueLength() == 5);
    REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(9 + 7 + 5 + 3 + 1));

    simulator::run(convertSecondsToTime(17.0));
    REQUIRE(reqHandler->numArrivals() == 8);
    REQUIRE(reqHandler->numCompletions() == 3);
    REQUIRE(reqHandler->getQueueLength() == 5);
    REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(9 + 7 + 5 + 3 + 1));

    simulator::run(convertSecondsToTime(19.0));
    REQUIRE(reqHandler->numArrivals() == 9);
    REQUIRE(reqHandler->numCompletions() == 4);
    REQUIRE(reqHandler->getQueueLength() == 5);
    REQUIRE(reqHandler->getRemainingWorkLeft() == convertSecondsToTime(9 + 7 + 5 + 3 + 1));

    Factory<RequestHandler>::finish();
    Factory<ArrivalProcess>::finish();
    Factory<RequestGenerator>::finish();
    Factory<PerfModel>::finish();
    Factory<Distribution>::finish();
    ObjectPool<Request>::finish();
    simulator::reset();
}
