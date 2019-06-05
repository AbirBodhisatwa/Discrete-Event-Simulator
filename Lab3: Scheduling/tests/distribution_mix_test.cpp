#include "catch/catch.hpp"
#include "json/json.h"
#include "../distribution_mix.h"
#include "../random_helper.h"
#include "../factory.h"

TEST_CASE("degenerate hyperexponential distribution has accurate first, second, and third moments", "[DistributionMix]") {
    double p;
    double lambda = 4.0;
    SECTION("p = 0.5")
    {
        p = 0.5;
    }
    SECTION("p = 0.25")
    {
        p = 0.25;
    }
    Json::Value config;
    config["dists"][0u]["name"] = "Exp";
    config["dists"][0u]["type"] = "DistributionExp";
    config["dists"][0u]["rate"] = p * lambda;
    config["dists"][1]["name"] = "Det";
    config["dists"][1]["type"] = "DistributionDet";
    config["dists"][1]["val"] = 0;
    config["probabilities"][0u] = p;
    config["probabilities"][1] = 1.0 - p;
    double EX = 1.0 / lambda;
    double EX2 = 2.0 / (p * lambda * lambda);
    double EX3 = 6.0 / (p * p * lambda * lambda * lambda);
    DistributionMix dist(config);
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
    REQUIRE(sum == Approx(EX).epsilon(0.01));
    // Second moment E[X^2]
    REQUIRE(sum2 == Approx(EX2).epsilon(0.01));
    // Third moment E[X^3]
    REQUIRE(sum3 == Approx(EX3).epsilon(0.01));

    Factory<Distribution>::finish();
}

TEST_CASE("bimodal distribution", "[DistributionMix]")
{
    double a = 3.0;
    double b = 7.0;
    double p;
    SECTION("p = 0.5")
    {
        p = 0.5;
    }
    SECTION("p = 0.25")
    {
        p = 0.25;
    }
    Json::Value config;
    config["dists"][0u]["name"] = "DetA";
    config["dists"][0u]["type"] = "DistributionDet";
    config["dists"][0u]["val"] = a;
    config["dists"][1]["name"] = "DetB";
    config["dists"][1]["type"] = "DistributionDet";
    config["dists"][1]["val"] = b;
    config["probabilities"][0u] = p;
    config["probabilities"][1] = 1.0 - p;
    double EX = p * a + (1.0 - p) * b;
    double EX2 = p * a * a + (1.0 - p) * b * b;
    double EX3 = p * a * a * a + (1.0 - p) * b * b * b;
    DistributionMix dist(config);
    double sum = 0;
    double sum2 = 0;
    double sum3 = 0;
    int n = 1000000;
    for (int i = 0; i < n; i++) {
        double r = dist.nextRand();
        REQUIRE(((r == a) || (r == b)));
        sum += r;
        sum2 += r * r;
        sum3 += r * r * r;
    }
    sum /= n;
    sum2 /= n;
    sum3 /= n;

    // First moment E[X]
    REQUIRE(sum == Approx(EX).epsilon(0.01));
    // Second moment E[X^2]
    REQUIRE(sum2 == Approx(EX2).epsilon(0.01));
    // Third moment E[X^3]
    REQUIRE(sum3 == Approx(EX3).epsilon(0.01));

    Factory<Distribution>::finish();
}

TEST_CASE("distribution mix repeatable with same seed", "[DistributionMix]") {
    double p = 0.5;
    double lambda = 4.0;
    Json::Value config;
    config["dists"][0u]["name"] = "Exp";
    config["dists"][0u]["type"] = "DistributionExp";
    config["dists"][0u]["rate"] = p * lambda;
    config["dists"][1]["name"] = "Det";
    config["dists"][1]["type"] = "DistributionDet";
    config["dists"][1]["val"] = 0;
    config["probabilities"][0u] = p;
    config["probabilities"][1] = 1.0 - p;
    seedGenerator(1234);
    DistributionMix dist(config);
    double v1 = dist.nextRand();
    double v2 = dist.nextRand();
    double v3 = dist.nextRand();

    Factory<Distribution>::finish();

    seedGenerator(1234);
    DistributionMix dist2(config);
    REQUIRE(v1 == dist2.nextRand());
    REQUIRE(v2 == dist2.nextRand());
    REQUIRE(v3 == dist2.nextRand());

    Factory<Distribution>::finish();
}
