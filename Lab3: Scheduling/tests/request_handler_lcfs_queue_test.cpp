#include "catch/catch.hpp"
#include "test_request_stream.h"

TEST_CASE("test lcfs scheduler", "[RequestHandlerLcfsQueue]") {
    Json::Value config;
    config["type"] = "RequestHandlerLcfsQueue";
    Json::Value reqs;
    addTestRequestStreamConfig(reqs, 0, 1, 1);
    addTestRequestStreamConfig(reqs, 0, 3, 6);
    addTestRequestStreamConfig(reqs, 0, 2, 3);
    addTestRequestStreamConfig(reqs, 7, 2, 9);
    addTestRequestStreamConfig(reqs, 8, 3, 13);
    addTestRequestStreamConfig(reqs, 8, 1, 10);
    testRequestHandler(config, reqs);
}
