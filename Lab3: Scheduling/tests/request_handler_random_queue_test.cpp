#include "catch/catch.hpp"
#include "test_request_stream.h"

TEST_CASE("test random scheduler", "[RequestHandlerRandomQueue]") {
    Json::Value config;
    config["type"] = "RequestHandlerRandomQueue";
    Json::Value reqs;
    addTestRequestStreamConfig(reqs, 0, 1, 1);
    addTestRequestStreamConfig(reqs, 0, 3, -1);
    addTestRequestStreamConfig(reqs, 0, 2, -1);
    addTestRequestStreamConfig(reqs, 7, 3, 10);
    addTestRequestStreamConfig(reqs, 7, 1, 11);
    testRequestHandler(config, reqs);
}
