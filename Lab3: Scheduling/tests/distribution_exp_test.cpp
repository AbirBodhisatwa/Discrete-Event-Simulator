#include "catch/catch.hpp"
#include "json/json.h"
#include "../distribution_exp.h"
#include "../random_helper.h"

TEST_CASE("exponential distribution has accurate first, second, and third moments", "[DistributionExp]") {
    Json::Value config;
    config["rate"] = 4.0;
    DistributionExp dist(config);

    double sum = 0;
    double sum2 = 0;
    double sum3 = 0;
    int n = 1000000;
    for (int i = 0; i < n; i++) {
        double r = dist.nextRand();
        sum += r;
        sum2 += r * r;
        sum3 += r * r * r;
    }
    sum /= n;
    sum2 /= n;
    sum3 /= n;

    // First moment E[X]
    REQUIRE(sum == Approx(0.25).epsilon(0.01));
    // Second moment E[X^2]
    REQUIRE(sum2 == Approx(0.125).epsilon(0.01));
    // Third moment E[X^3]
    REQUIRE(sum3 == Approx(0.09375).epsilon(0.01));
}

TEST_CASE("exponential distribution repeatable with same seed", "[DistributionExp]") {
    seedGenerator(1234);
    Json::Value config;
    config["rate"] = 4.0;
    DistributionExp dist(config);
    double v1 = dist.nextRand();
    double v2 = dist.nextRand();
    double v3 = dist.nextRand();

    seedGenerator(1234);
    DistributionExp dist2(config);
    REQUIRE(v1 == dist2.nextRand());
    REQUIRE(v2 == dist2.nextRand());
    REQUIRE(v3 == dist2.nextRand());
}
