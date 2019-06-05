#include "catch/catch.hpp"
#include "test_request_stream.h"

TEST_CASE("test central queue scheduler with k servers", "[RequestHandlerCentralQueueK]") {
    Json::Value config;
    config["type"] = "RequestHandlerCentralQueueK";
    config["k"] = 2;
    Json::Value reqs;
    addTestRequestStreamConfig(reqs, 0, 1, 1);
    addTestRequestStreamConfig(reqs, 0, 3, 3);
    addTestRequestStreamConfig(reqs, 0, 2, 3);
    addTestRequestStreamConfig(reqs, 7, 1, 8);
    addTestRequestStreamConfig(reqs, 7, 2, 9);
    addTestRequestStreamConfig(reqs, 7, 3, 11);
    testRequestHandler(config, reqs);
}
