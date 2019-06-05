#include <vector>
#include "catch/catch.hpp"
#include "json/json.h"
#include "../request_handler_dispatcher_least_work_left.h"
#include "../request_handler.h"
#include "../request_generator.h"
#include "../distribution.h"
#include "../request.h"
#include "../time_helper.h"
#include "../simulator.h"
#include "../factory.h"
#include "../object_pool.h"
#include "test_request_handler.h"

TEST_CASE("test least work left dispatcher", "[RequestHandlerDispatcherLeastWorkLeft]") {
    simulator::reset();
    TestRequestHandler* reqHandler1 = createTestRequestHandler("testRequestHandler1", 100);
    TestRequestHandler* reqHandler2 = createTestRequestHandler("testRequestHandler2", 100);
    TestRequestHandler* reqHandler3 = createTestRequestHandler("testRequestHandler3", 100);
    std::vector<RequestHandler*> reqHandlers;
    reqHandlers.push_back(reqHandler1);
    reqHandlers.push_back(reqHandler2);
    reqHandlers.push_back(reqHandler3);
    Json::Value config;
    config["reqHandlers"][0u] = "testRequestHandler1";
    config["reqHandlers"][1] = "testRequestHandler2";
    config["reqHandlers"][2] = "testRequestHandler3";
    RequestHandlerDispatcherLeastWorkLeft dispatcher(config);
    dispatcher.init();
    CompletionRequestDestroy completion;

    reqHandler1->handleRequest(Request::create(0, 5000), &completion);
    reqHandler2->handleRequest(Request::create(0, 3000), &completion);
    reqHandler3->handleRequest(Request::create(0, 1000), &completion);
    REQUIRE(reqHandler1->numArrivals() == 1);
    REQUIRE(reqHandler1->numCompletions() == 0);
    REQUIRE(reqHandler1->getQueueLength() == reqHandler1->numArrivals());
    REQUIRE(reqHandler1->getRemainingWorkLeft() == convertSecondsToTime(5 * 10));
    REQUIRE(reqHandler2->numArrivals() == 1);
    REQUIRE(reqHandler2->numCompletions() == 0);
    REQUIRE(reqHandler2->getQueueLength() == reqHandler2->numArrivals());
    REQUIRE(reqHandler2->getRemainingWorkLeft() == convertSecondsToTime(3 * 10));
    REQUIRE(reqHandler3->numArrivals() == 1);
    REQUIRE(reqHandler3->numCompletions() == 0);
    REQUIRE(reqHandler3->getQueueLength() == reqHandler3->numArrivals());
    REQUIRE(reqHandler3->getRemainingWorkLeft() == convertSecondsToTime(1 * 10));

    Request* req = Request::create(0, 1000);
    REQUIRE(dispatcher.selectRequestHandler(req, reqHandlers) == 2);
    dispatcher.handleRequest(req, &completion);
    REQUIRE(reqHandler3->numArrivals() == 2);
    REQUIRE(reqHandler3->numCompletions() == 0);
    REQUIRE(reqHandler3->getQueueLength() == reqHandler3->numArrivals());
    REQUIRE(reqHandler3->getRemainingWorkLeft() == convertSecondsToTime(2 * 10));
    req = Request::create(0, 1000);
    REQUIRE(reqHandlers[0]->getRemainingWorkLeft() == convertSecondsToTime(5 * 10));
    REQUIRE(reqHandlers[1]->getRemainingWorkLeft() == convertSecondsToTime(3 * 10));
    REQUIRE(reqHandlers[2]->getRemainingWorkLeft() == convertSecondsToTime(2 * 10));
    REQUIRE(dispatcher.selectRequestHandler(req, reqHandlers) == 2);
    dispatcher.handleRequest(req, &completion);
    REQUIRE(reqHandler3->numArrivals() == 3);
    REQUIRE(reqHandler3->numCompletions() == 0);
    REQUIRE(reqHandler3->getQueueLength() == reqHandler3->numArrivals());
    REQUIRE(reqHandler3->getRemainingWorkLeft() == convertSecondsToTime(3 * 10));

    req = Request::create(0, 1000);
    REQUIRE(dispatcher.selectRequestHandler(req, reqHandlers) != 0);
    dispatcher.handleRequest(req, &completion);
    req = Request::create(0, 1000);
    REQUIRE(dispatcher.selectRequestHandler(req, reqHandlers) != 0);
    dispatcher.handleRequest(req, &completion);
    REQUIRE(reqHandler2->numArrivals() == 2);
    REQUIRE(reqHandler2->numCompletions() == 0);
    REQUIRE(reqHandler2->getQueueLength() == reqHandler2->numArrivals());
    REQUIRE(reqHandler2->getRemainingWorkLeft() == convertSecondsToTime(4 * 10));
    REQUIRE(reqHandler3->numArrivals() == 4);
    REQUIRE(reqHandler3->numCompletions() == 0);
    REQUIRE(reqHandler3->getQueueLength() == reqHandler3->numArrivals());
    REQUIRE(reqHandler3->getRemainingWorkLeft() == convertSecondsToTime(4 * 10));
    req = Request::create(0, 2000);
    REQUIRE(dispatcher.selectRequestHandler(req, reqHandlers) != 0);
    dispatcher.handleRequest(req, &completion);
    req = Request::create(0, 2000);
    REQUIRE(dispatcher.selectRequestHandler(req, reqHandlers) != 0);
    dispatcher.handleRequest(req, &completion);
    REQUIRE(reqHandler2->numArrivals() == 3);
    REQUIRE(reqHandler2->numCompletions() == 0);
    REQUIRE(reqHandler2->getQueueLength() == reqHandler2->numArrivals());
    REQUIRE(reqHandler2->getRemainingWorkLeft() == convertSecondsToTime(6 * 10));
    REQUIRE(reqHandler3->numArrivals() == 5);
    REQUIRE(reqHandler3->numCompletions() == 0);
    REQUIRE(reqHandler3->getQueueLength() == reqHandler3->numArrivals());
    REQUIRE(reqHandler3->getRemainingWorkLeft() == convertSecondsToTime(6 * 10));
    req = Request::create(0, 1000);
    REQUIRE(dispatcher.selectRequestHandler(req, reqHandlers) == 0);
    dispatcher.handleRequest(req, &completion);
    REQUIRE(reqHandler1->numArrivals() == 2);
    REQUIRE(reqHandler1->numCompletions() == 0);
    REQUIRE(reqHandler1->getQueueLength() == reqHandler1->numArrivals());
    REQUIRE(reqHandler1->getRemainingWorkLeft() == convertSecondsToTime(6 * 10));

    dispatcher.handleRequest(Request::create(0, 1000), &completion);
    dispatcher.handleRequest(Request::create(0, 1000), &completion);
    dispatcher.handleRequest(Request::create(0, 1000), &completion);
    REQUIRE(reqHandler1->numArrivals() == 3);
    REQUIRE(reqHandler1->numCompletions() == 0);
    REQUIRE(reqHandler1->getQueueLength() == reqHandler1->numArrivals());
    REQUIRE(reqHandler1->getRemainingWorkLeft() == convertSecondsToTime(7 * 10));
    REQUIRE(reqHandler2->numArrivals() == 4);
    REQUIRE(reqHandler2->numCompletions() == 0);
    REQUIRE(reqHandler2->getQueueLength() == reqHandler2->numArrivals());
    REQUIRE(reqHandler2->getRemainingWorkLeft() == convertSecondsToTime(7 * 10));
    REQUIRE(reqHandler3->numArrivals() == 6);
    REQUIRE(reqHandler3->numCompletions() == 0);
    REQUIRE(reqHandler3->getQueueLength() == reqHandler3->numArrivals());
    REQUIRE(reqHandler3->getRemainingWorkLeft() == convertSecondsToTime(7 * 10));

    dispatcher.handleRequest(Request::create(0, 1000), &completion);
    dispatcher.handleRequest(Request::create(0, 5000), &completion);
    dispatcher.handleRequest(Request::create(0, 3000), &completion);
    dispatcher.handleRequest(Request::create(0, 5000), &completion);
    dispatcher.handleRequest(Request::create(0, 3000), &completion);
    dispatcher.handleRequest(Request::create(0, 1000), &completion);
    REQUIRE(reqHandler1->numArrivals() == 5);
    REQUIRE(reqHandler1->numCompletions() == 0);
    REQUIRE(reqHandler1->getQueueLength() == reqHandler1->numArrivals());
    REQUIRE(reqHandler1->getRemainingWorkLeft() == convertSecondsToTime(13 * 10));
    REQUIRE(reqHandler2->numArrivals() == 6);
    REQUIRE(reqHandler2->numCompletions() == 0);
    REQUIRE(reqHandler2->getQueueLength() == reqHandler2->numArrivals());
    REQUIRE(reqHandler2->getRemainingWorkLeft() == convertSecondsToTime(13 * 10));
    REQUIRE(reqHandler3->numArrivals() == 8);
    REQUIRE(reqHandler3->numCompletions() == 0);
    REQUIRE(reqHandler3->getQueueLength() == reqHandler3->numArrivals());
    REQUIRE(reqHandler3->getRemainingWorkLeft() == convertSecondsToTime(13 * 10));

    simulator::run();

    Factory<RequestHandler>::finish();
    Factory<PerfModel>::finish();
    Factory<Distribution>::finish();
    ObjectPool<Request>::finish();
    simulator::reset();
}
