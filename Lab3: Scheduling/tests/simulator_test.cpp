#include "catch/catch.hpp"
#include "json/json.h"
#include "../simulator.h"

int callback1Calls = 0;
int callback2Calls = 0;
int callback3Calls = 0;
void testEventCallback1(const Event& e)
{
    REQUIRE(simulator::getSimTime() == e.timestamp);
    callback1Calls++;
}
void testEventCallback2(const Event& e)
{
    REQUIRE(simulator::getSimTime() == e.timestamp);
    callback2Calls++;
}
void testEventCallback3(const Event& e)
{
    REQUIRE(simulator::getSimTime() == e.timestamp);
    callback3Calls++;
}

TEST_CASE("test simulator", "[simulator]") {
    callback1Calls = 0;
    callback2Calls = 0;
    callback3Calls = 0;
    simulator::reset();

    Event e;
    e.timestamp = 100;
    e.callback = testEventCallback1;
    simulator::schedule(e);

    e.timestamp = 200;
    e.callback = testEventCallback2;
    simulator::schedule(e);

    e.timestamp = 400;
    e.callback = testEventCallback1;
    simulator::schedule(e);

    e.timestamp = 600;
    e.callback = testEventCallback1;
    simulator::schedule(e);

    e.timestamp = 50;
    e.callback = testEventCallback3;
    simulator::schedule(e);

    e.timestamp = 150;
    e.callback = testEventCallback3;
    simulator::schedule(e);

    e.timestamp = 250;
    e.callback = testEventCallback3;
    simulator::schedule(e);

    e.timestamp = 350;
    e.callback = testEventCallback3;
    simulator::schedule(e);

    e.timestamp = 450;
    e.callback = testEventCallback3;
    simulator::schedule(e);

    e.timestamp = 550;
    e.callback = testEventCallback3;
    simulator::schedule(e);

    SECTION("test simulator run") {
        simulator::run();
        REQUIRE(simulator::getSimTime() == 600);
        REQUIRE(callback1Calls == 3);
        REQUIRE(callback2Calls == 1);
        REQUIRE(callback3Calls == 6);
    }

    SECTION("test simulator run with stop time") {
        simulator::run(300);
        REQUIRE(simulator::getSimTime() == 300);
        REQUIRE(callback1Calls == 1);
        REQUIRE(callback2Calls == 1);
        REQUIRE(callback3Calls == 3);

        SECTION("test simulator continuing run") {
            simulator::run();
            REQUIRE(simulator::getSimTime() == 600);
            REQUIRE(callback1Calls == 3);
            REQUIRE(callback2Calls == 1);
            REQUIRE(callback3Calls == 6);
        }

        SECTION("test simulator continuing run with stop time") {
            simulator::run(500);
            REQUIRE(simulator::getSimTime() == 500);
            REQUIRE(callback1Calls == 2);
            REQUIRE(callback2Calls == 1);
            REQUIRE(callback3Calls == 5);

            SECTION("test simulator reset") {
                callback1Calls = 0;
                callback2Calls = 0;
                callback3Calls = 0;
                simulator::reset();
                REQUIRE(simulator::getSimTime() == 0);
                REQUIRE(callback1Calls == 0);
                REQUIRE(callback2Calls == 0);
                REQUIRE(callback3Calls == 0);
                simulator::run();
                REQUIRE(simulator::getSimTime() == 0);
                REQUIRE(callback1Calls == 0);
                REQUIRE(callback2Calls == 0);
                REQUIRE(callback3Calls == 0);
            }
        }
    }

    simulator::reset();
}

TEST_CASE("test simulator removeEvent", "[simulator]") {
    callback1Calls = 0;
    callback2Calls = 0;
    callback3Calls = 0;
    simulator::reset();

    Event e;
    e.timestamp = 100;
    e.callback = testEventCallback1;
    simulator::EventReference e1 = simulator::schedule(e);

    e.timestamp = 200;
    e.callback = testEventCallback2;
    simulator::EventReference e2 = simulator::schedule(e);

    SECTION("test simulator removeEvent case 1") {
        simulator::removeEvent(e1);
        simulator::removeEvent(e2);
        simulator::run();
        REQUIRE(simulator::getSimTime() == 0);
        REQUIRE(callback1Calls == 0);
        REQUIRE(callback2Calls == 0);
        REQUIRE(callback3Calls == 0);
    }

    SECTION("test simulator removeEvent case 2") {
        simulator::removeEvent(e1);
        simulator::run();
        REQUIRE(simulator::getSimTime() == 200);
        REQUIRE(callback1Calls == 0);
        REQUIRE(callback2Calls == 1);
        REQUIRE(callback3Calls == 0);
    }

    SECTION("test simulator removeEvent case 3") {
        simulator::removeEvent(e2);
        simulator::run();
        REQUIRE(simulator::getSimTime() == 100);
        REQUIRE(callback1Calls == 1);
        REQUIRE(callback2Calls == 0);
        REQUIRE(callback3Calls == 0);
    }

    SECTION("test simulator removeEvent case 4") {
        e.timestamp = 400;
        e.callback = testEventCallback1;
        simulator::schedule(e);

        e.timestamp = 600;
        e.callback = testEventCallback1;
        simulator::EventReference e3 = simulator::schedule(e);

        e.timestamp = 50;
        e.callback = testEventCallback3;
        simulator::schedule(e);

        e.timestamp = 150;
        e.callback = testEventCallback3;
        simulator::EventReference e4 = simulator::schedule(e);

        e.timestamp = 250;
        e.callback = testEventCallback3;
        simulator::schedule(e);

        simulator::removeEvent(e4);
        simulator::removeEvent(e3);
        simulator::removeEvent(e1);
        simulator::removeEvent(e2);
        simulator::run();
        REQUIRE(simulator::getSimTime() == 400);
        REQUIRE(callback1Calls == 1);
        REQUIRE(callback2Calls == 0);
        REQUIRE(callback3Calls == 2);
    }

    simulator::reset();
}
