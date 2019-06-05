#include "catch/catch.hpp"
#include "test_request_stream.h"

TEST_CASE("test fb scheduler", "[RequestHandlerFbQueue]") {
    Json::Value config;
    config["type"] = "RequestHandlerFbQueue";
    Json::Value reqs;
    addTestRequestStreamConfig(reqs, 0, 1, 3);
    addTestRequestStreamConfig(reqs, 0, 3, 6);
    addTestRequestStreamConfig(reqs, 0, 2, 5);
    addTestRequestStreamConfig(reqs, 7, 3, 13);
    addTestRequestStreamConfig(reqs, 9, 2, 12);
    addTestRequestStreamConfig(reqs, 10, 1, 11);
    testRequestHandler(config, reqs);
}
