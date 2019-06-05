#include "catch/catch.hpp"
#include "test_request_stream.h"

TEST_CASE("test psjf scheduler", "[RequestHandlerPsjfQueue]") {
    Json::Value config;
    config["type"] = "RequestHandlerPsjfQueue";
    Json::Value reqs;
    addTestRequestStreamConfig(reqs, 0, 3, 6);
    addTestRequestStreamConfig(reqs, 0, 1, 1);
    addTestRequestStreamConfig(reqs, 0, 2, 3);
    addTestRequestStreamConfig(reqs, 7, 3, 13);
    addTestRequestStreamConfig(reqs, 8, 2, 11);
    addTestRequestStreamConfig(reqs, 9.5, 1, 10.5);
    testRequestHandler(config, reqs);
}
