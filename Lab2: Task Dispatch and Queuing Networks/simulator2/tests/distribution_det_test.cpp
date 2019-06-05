#include "catch/catch.hpp"
#include "json/json.h"
#include "../distribution_det.h"

TEST_CASE("deterministic dist returns constant", "[DistributionDet]") {
    Json::Value config;
    config["val"] = 7.0;
    DistributionDet dist(config);

    REQUIRE(dist.nextRand() == 7.0);
    REQUIRE(dist.nextRand() == 7.0);
    REQUIRE(dist.nextRand() == 7.0);
}
