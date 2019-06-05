#include "catch/catch.hpp"
#include "test_request_stream.h"

TEST_CASE("test plcfs scheduler", "[RequestHandlerPlcfsQueue]") {
    Json::Value config;
    config["type"] = "RequestHandlerPlcfsQueue";
    Json::Value reqs;
    addTestRequestStreamConfig(reqs, 0, 1, 6);
    addTestRequestStreamConfig(reqs, 0, 3, 5);
    addTestRequestStreamConfig(reqs, 0, 2, 2);
    addTestRequestStreamConfig(reqs, 7, 2, 13);
    addTestRequestStreamConfig(reqs, 8, 3, 12);
    addTestRequestStreamConfig(reqs, 8, 1, 9);
    testRequestHandler(config, reqs);
}
