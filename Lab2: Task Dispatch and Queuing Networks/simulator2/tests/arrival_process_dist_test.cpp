#include "catch/catch.hpp"
#include "json/json.h"
#include "../arrival_process_dist.h"
#include "../time_helper.h"
#include "../factory.h"

TEST_CASE("nextArrival properly handles current time parameter and time unit conversions", "[ArrivalProcessDist]") {
    Json::Value config;
    config["dist"]["name"] = "1.0sec_dist";
    config["dist"]["type"] = "DistributionDet";
    config["dist"]["val"] = 1.0;
    ArrivalProcessDist arrivalProcess(config);

    REQUIRE(arrivalProcess.nextArrival(convertSecondsToTime(0)) == convertSecondsToTime(1.0));
    REQUIRE(arrivalProcess.nextArrival(convertSecondsToTime(1.0)) == convertSecondsToTime(2.0));
    REQUIRE(arrivalProcess.nextArrival(convertSecondsToTime(1.5)) == convertSecondsToTime(2.5));
    REQUIRE(arrivalProcess.nextArrival(convertSecondsToTime(15.0)) == convertSecondsToTime(16.0));

    Factory<Distribution>::finish();
}
