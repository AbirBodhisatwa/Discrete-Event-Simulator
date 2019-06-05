#include "catch/catch.hpp"
#include "test_request_stream.h"

TEST_CASE("test sjf scheduler", "[RequestHandlerSjfQueue]") {
    Json::Value config;
    config["type"] = "RequestHandlerSjfQueue";
    Json::Value reqs;
    addTestRequestStreamConfig(reqs, 0, 1, 1);
    addTestRequestStreamConfig(reqs, 0, 3, 6);
    addTestRequestStreamConfig(reqs, 0, 2, 3);
    addTestRequestStreamConfig(reqs, 7, 3, 10);
    addTestRequestStreamConfig(reqs, 8, 2, 13);
    addTestRequestStreamConfig(reqs, 9, 1, 11);
    testRequestHandler(config, reqs);
}
