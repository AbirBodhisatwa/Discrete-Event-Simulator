#include "catch/catch.hpp"
#include "json/json.h"
#include "../request_generator_dist.h"
#include "../factory.h"
#include "../object_pool.h"
#include "../request.h"

TEST_CASE("nextRequest properly handles arrival times and request sizes", "[RequestGeneratorDist]") {
    Json::Value config;
    config["dist"]["name"] = "100bytes_dist";
    config["dist"]["type"] = "DistributionDet";
    config["dist"]["val"] = 100.0;
    RequestGeneratorDist requestGenerator(config);

    Request* req1 = requestGenerator.nextRequest(0);
    REQUIRE(req1);
    REQUIRE(req1->getArrivalTime() == 0);
    REQUIRE(req1->getSize() == 100);
    req1->destroy();

    Request* req2 = requestGenerator.nextRequest(1234);
    REQUIRE(req2 == req1); // ensure request object is reused
    REQUIRE(req2->getArrivalTime() == 1234);
    REQUIRE(req2->getSize() == 100);
    req2->destroy();

    Request* req3 = requestGenerator.nextRequest(789);
    REQUIRE(req3 == req1); // ensure request object is reused
    REQUIRE(req3->getArrivalTime() == 789);
    REQUIRE(req3->getSize() == 100);
    req3->destroy();

    Factory<Distribution>::finish();
    ObjectPool<Request>::finish();
}
