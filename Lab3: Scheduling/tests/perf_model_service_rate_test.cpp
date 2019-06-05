#include "catch/catch.hpp"
#include "json/json.h"
#include "../perf_model_service_rate.h"
#include "../time_helper.h"
#include "../request.h"

TEST_CASE("estimateWork properly estimates work based on the request size with the correct time unit conversions", "[PerfModelServiceRate]") {
    Json::Value config;
    config["serviceRate"] = 2.0; // bytes/sec
    PerfModelServiceRate perfModel(config);
    Request* req = Request::create(0, 7);
    REQUIRE(perfModel.estimateWork(req) == convertSecondsToTime(3.5));
    req->setSize(3);
    REQUIRE(perfModel.estimateWork(req) == convertSecondsToTime(1.5));
    req->setSize(2);
    req->setRemainingWork(&perfModel);
    REQUIRE(req->getRemainingWork() == convertSecondsToTime(1.0));

    req->destroy();
}
