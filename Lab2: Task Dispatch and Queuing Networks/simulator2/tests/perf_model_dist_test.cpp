#include "catch/catch.hpp"
#include "json/json.h"
#include "../perf_model_dist.h"
#include "../time_helper.h"
#include "../request.h"
#include "../factory.h"
#include "../distribution.h"

TEST_CASE("estimateWork properly ignores the request size and uses the distribution to estimate work with the correct time unit conversions", "[PerfModelDist]") {
    Json::Value config;
    config["dist"]["name"] = "1.0sec_dist";
    config["dist"]["type"] = "DistributionDet";
    config["dist"]["val"] = 1.0;
    PerfModelDist perfModel(config);
    Request* req = Request::create(0, 7);
    REQUIRE(perfModel.estimateWork(req) == convertSecondsToTime(1.0));
    req->setSize(3);
    REQUIRE(perfModel.estimateWork(req) == convertSecondsToTime(1.0));
    req->setRemainingWork(&perfModel);
    REQUIRE(req->getRemainingWork() == convertSecondsToTime(1.0));

    req->destroy();
    Factory<Distribution>::finish();
}
