#include "catch/catch.hpp"
#include "test_request_stream.h"

TEST_CASE("test ps scheduler", "[RequestHandlerPsQueue]") {
    Json::Value config;
    config["type"] = "RequestHandlerPsQueue";
    Json::Value reqs;
    addTestRequestStreamConfig(reqs, 0, 1, 3);
    addTestRequestStreamConfig(reqs, 0, 3, 6);
    addTestRequestStreamConfig(reqs, 0, 2, 5);
    addTestRequestStreamConfig(reqs, 7, 1, 9.5);
    addTestRequestStreamConfig(reqs, 7, 3, 13);
    addTestRequestStreamConfig(reqs, 8, 2, 12.5);
    testRequestHandler(config, reqs);
}
