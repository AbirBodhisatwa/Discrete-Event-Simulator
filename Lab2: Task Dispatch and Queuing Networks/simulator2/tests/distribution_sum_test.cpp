#include "catch/catch.hpp"
#include "json/json.h"
#include "../distribution_sum.h"
#include "../random_helper.h"
#include "../factory.h"

TEST_CASE("Erlang-k distribution has accurate first, second, and third moments", "[DistributionSum]") {
    unsigned int k;
    double lambda = 4.0;
    SECTION("k = 2")
    {
        k = 2;
    }
    SECTION("k = 4")
    {
        k = 4;
    }
    Json::Value configExp;
    configExp["name"] = "Exp";
    configExp["type"] = "DistributionExp";
    configExp["rate"] = lambda * k;
    Factory<Distribution>::create(configExp);
    Json::Value config;
    for (unsigned int i = 0; i < k; i++) {
        config["dists"][i] = configExp["name"];
    }
    double EX = 1.0 / lambda;
    double EX2 = (1.0 + 1.0 / static_cast<double>(k)) / (lambda * lambda);
    double EX3 = (1.0 + 3.0 / static_cast<double>(k) + 2.0 / static_cast<double>(k * k)) / (lambda * lambda * lambda);
    DistributionSum dist(config);
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

TEST_CASE("shifted exponential distribution", "[DistributionSum]")
{
    double shift = 5.0;
    double lambda = 4.0;
    Json::Value config;
    config["dists"][0u]["name"] = "Det";
    config["dists"][0u]["type"] = "DistributionDet";
    config["dists"][0u]["val"] = shift;
    config["dists"][1]["name"] = "Exp";
    config["dists"][1]["type"] = "DistributionExp";
    config["dists"][1]["rate"] = lambda;
    double EX = shift + 1.0 / lambda;
    double EX2 = 1.0 / (lambda * lambda) + EX * EX;
    double EX3 = 2.0 / (lambda * lambda * lambda) + 3.0 * EX / (lambda * lambda) + EX * EX * EX;
    DistributionSum dist(config);
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

TEST_CASE("deterministic sum distribution", "[DistributionSum]")
{
    double a = 3.0;
    double b = 7.0;
    Json::Value config;
    config["dists"][0u]["name"] = "DetA";
    config["dists"][0u]["type"] = "DistributionDet";
    config["dists"][0u]["val"] = a;
    config["dists"][1]["name"] = "DetB";
    config["dists"][1]["type"] = "DistributionDet";
    config["dists"][1]["val"] = b;
    DistributionSum dist(config);
    REQUIRE(dist.nextRand() == (a + b));
    REQUIRE(dist.nextRand() == (a + b));
    REQUIRE(dist.nextRand() == (a + b));

    Factory<Distribution>::finish();
}

TEST_CASE("distribution sum repeatable with same seed", "[DistributionSum]") {
    double shift = 5.0;
    double lambda = 4.0;
    Json::Value config;
    config["dists"][0u]["name"] = "Det";
    config["dists"][0u]["type"] = "DistributionDet";
    config["dists"][0u]["val"] = shift;
    config["dists"][1]["name"] = "Exp";
    config["dists"][1]["type"] = "DistributionExp";
    config["dists"][1]["rate"] = lambda;
    seedGenerator(1234);
    DistributionSum dist(config);
    double v1 = dist.nextRand();
    double v2 = dist.nextRand();
    double v3 = dist.nextRand();

    Factory<Distribution>::finish();

    seedGenerator(1234);
    DistributionSum dist2(config);
    REQUIRE(v1 == dist2.nextRand());
    REQUIRE(v2 == dist2.nextRand());
    REQUIRE(v3 == dist2.nextRand());

    Factory<Distribution>::finish();
}
