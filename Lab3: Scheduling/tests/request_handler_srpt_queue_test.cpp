#include "catch/catch.hpp"
#include "test_request_stream.h"

TEST_CASE("test srpt scheduler", "[RequestHandlerSrptQueue]") {
    Json::Value config;
    config["type"] = "RequestHandlerSrptQueue";
    Json::Value reqs;
    addTestRequestStreamConfig(reqs, 0, 3, 4);
    addTestRequestStreamConfig(reqs, 1, 1, 2);
    addTestRequestStreamConfig(reqs, 3, 2, 6);
    addTestRequestStreamConfig(reqs, 7, 3, 11);
    addTestRequestStreamConfig(reqs, 8.5, 2, 13);
    addTestRequestStreamConfig(reqs, 8.5, 1, 9.5);
    testRequestHandler(config, reqs);
}
