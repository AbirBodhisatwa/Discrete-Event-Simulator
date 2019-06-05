#include "catch/catch.hpp"
#include "test_request_stream.h"

TEST_CASE("test fifo scheduler", "[RequestHandlerFifoQueue]") {
    Json::Value config;
    config["type"] = "RequestHandlerFifoQueue";
    Json::Value reqs;
    addTestRequestStreamConfig(reqs, 0, 1, 1);
    addTestRequestStreamConfig(reqs, 0, 3, 4);
    addTestRequestStreamConfig(reqs, 0, 2, 6);
    addTestRequestStreamConfig(reqs, 7, 1, 8);
    addTestRequestStreamConfig(reqs, 7, 3, 11);
    addTestRequestStreamConfig(reqs, 7, 2, 13);
    testRequestHandler(config, reqs);
}
