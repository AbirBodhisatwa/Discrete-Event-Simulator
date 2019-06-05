#include "catch/catch.hpp"
#include "json/json.h"
#include "../request_handler_dispatcher_round_robin.h"
#include "../request_handler.h"
#include "../request_generator.h"
#include "../distribution.h"
#include "../request.h"
#include "../time_helper.h"
#include "../simulator.h"
#include "../factory.h"
#include "../object_pool.h"
#include "test_request_handler.h"

TEST_CASE("test round robin dispatcher", "[RequestHandlerDispatcherRoundRobin]") {
    simulator::reset();
    TestRequestHandler* reqHandler1 = createTestRequestHandler("testRequestHandler1", 100);
    TestRequestHandler* reqHandler2 = createTestRequestHandler("testRequestHandler2", 100);
    TestRequestHandler* reqHandler3 = createTestRequestHandler("testRequestHandler3", 100);
    Json::Value config;
    config["reqHandlers"][0u] = "testRequestHandler1";
    config["reqHandlers"][1] = "testRequestHandler2";
    config["reqHandlers"][2] = "testRequestHandler3";
    RequestHandlerDispatcherRoundRobin dispatcher(config);
    dispatcher.init();
    CompletionRequestDestroy completion;

    for (unsigned int i = 1; i <= 1000; i++) {
        for (unsigned int j = 0; j < 3; j++) {
            Request* req = Request::create(0, 1000);
            dispatcher.handleRequest(req, &completion);
        }
        REQUIRE(reqHandler1->numArrivals() == i);
        REQUIRE(reqHandler1->numCompletions() == 0);
        REQUIRE(reqHandler1->getQueueLength() == i);
        REQUIRE(reqHandler1->getRemainingWorkLeft() == convertSecondsToTime(i * 10));
        REQUIRE(reqHandler2->numArrivals() == i);
        REQUIRE(reqHandler2->numCompletions() == 0);
        REQUIRE(reqHandler2->getQueueLength() == i);
        REQUIRE(reqHandler2->getRemainingWorkLeft() == convertSecondsToTime(i * 10));
        REQUIRE(reqHandler3->numArrivals() == i);
        REQUIRE(reqHandler3->numCompletions() == 0);
        REQUIRE(reqHandler3->getQueueLength() == i);
        REQUIRE(reqHandler3->getRemainingWorkLeft() == convertSecondsToTime(i * 10));
    }

    simulator::run();

    Factory<RequestHandler>::finish();
    Factory<PerfModel>::finish();
    Factory<Distribution>::finish();
    ObjectPool<Request>::finish();
    simulator::reset();
}
