#include "catch/catch.hpp"
#include "json/json.h"
#include "../request_handler_dispatcher_random.h"
#include "../request_handler.h"
#include "../request_generator.h"
#include "../distribution.h"
#include "../request.h"
#include "../time_helper.h"
#include "../simulator.h"
#include "../factory.h"
#include "../object_pool.h"
#include "test_request_handler.h"

TEST_CASE("test random dispatcher", "[RequestHandlerDispatcherRandom]") {
    simulator::reset();
    TestRequestHandler* reqHandler1 = createTestRequestHandler("testRequestHandler1", 100);
    TestRequestHandler* reqHandler2 = createTestRequestHandler("testRequestHandler2", 100);
    TestRequestHandler* reqHandler3 = createTestRequestHandler("testRequestHandler3", 100);
    Json::Value config;
    config["reqHandlers"][0u] = "testRequestHandler1";
    config["reqHandlers"][1] = "testRequestHandler2";
    config["reqHandlers"][2] = "testRequestHandler3";
    RequestHandlerDispatcherRandom dispatcher(config);
    dispatcher.init();
    CompletionRequestDestroy completion;

    for (unsigned int i = 1; i <= 3000; i++) {
        Request* req = Request::create(0, 1000);
        dispatcher.handleRequest(req, &completion);
    }
    REQUIRE(reqHandler1->numArrivals() >= 980);
    REQUIRE(reqHandler1->numArrivals() <= 1020);
    REQUIRE(reqHandler1->numCompletions() == 0);
    REQUIRE(reqHandler1->getQueueLength() == reqHandler1->numArrivals());
    REQUIRE(reqHandler1->getRemainingWorkLeft() == convertSecondsToTime(reqHandler1->numArrivals() * 10));
    REQUIRE(reqHandler2->numArrivals() >= 980);
    REQUIRE(reqHandler2->numArrivals() <= 1020);
    REQUIRE(reqHandler2->numCompletions() == 0);
    REQUIRE(reqHandler2->getQueueLength() == reqHandler2->numArrivals());
    REQUIRE(reqHandler2->getRemainingWorkLeft() == convertSecondsToTime(reqHandler2->numArrivals() * 10));
    REQUIRE(reqHandler3->numArrivals() >= 980);
    REQUIRE(reqHandler3->numArrivals() <= 1020);
    REQUIRE(reqHandler3->numCompletions() == 0);
    REQUIRE(reqHandler3->getQueueLength() == reqHandler3->numArrivals());
    REQUIRE(reqHandler3->getRemainingWorkLeft() == convertSecondsToTime(reqHandler3->numArrivals() * 10));

    simulator::run();

    Factory<RequestHandler>::finish();
    Factory<PerfModel>::finish();
    Factory<Distribution>::finish();
    ObjectPool<Request>::finish();
    simulator::reset();
}
