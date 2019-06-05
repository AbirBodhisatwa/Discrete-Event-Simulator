#include "test_request_stream.h"
#include "catch/catch.hpp"
#include "../request_stream.h"
#include "../request.h"
#include "../listener.h"
#include "../distribution.h"
#include "../simulator.h"
#include "../time_helper.h"
#include "../factory.h"

REGISTER_CLASS(RequestStream, TestRequestStream)

TestRequestStream::TestRequestStream(Json::Value& config)
    : RequestStream(config),
      _config(config),
      _index(0)
{
}

TestRequestStream::~TestRequestStream()
{
}

// Initialize request stream
void TestRequestStream::init()
{
    RequestStream::init();
    addArrivalEvent();
}

// Next request in stream
Request* TestRequestStream::next()
{
    if (_index < _config["requests"].size()) {
        Json::Value& reqInfo = _config["requests"][_index];
        Request* req = Request::create(convertSecondsToTime(reqInfo["arrivalTime"].asDouble()), convertSecondsToTime(reqInfo["size"].asDouble()));
        if (reqInfo.isMember("completionTime")) {
            _completionTimesMap[req] = convertSecondsToTime(reqInfo["completionTime"].asDouble());
        }
        _index++;
        return req;
    } else {
        return NULL;
    }
}

// Called when request arrives
void TestRequestStream::notifyStart(Request* req)
{
    addArrivalEvent();
    RequestStream::notifyStart(req);
}

// Called when request is completed at request handler
void TestRequestStream::notifyEnd(Request* req)
{
    std::unordered_map<Request*, uint64_t>::iterator it = _completionTimesMap.find(req);
    if (it != _completionTimesMap.end()) {
        REQUIRE(it->second == simulator::getSimTime());
    }
    RequestStream::notifyEnd(req);
}

void testRequestHandler(Json::Value& config, Json::Value& reqs)
{
    simulator::reset();
    config["name"] = "TestRequestHandler";
    config["perfModel"]["name"] = "ServiceRate1000000000";
    config["perfModel"]["type"] = "PerfModelServiceRate";
    config["perfModel"]["serviceRate"] = 1000000000.0;
    RequestHandler* reqHandler = Factory<RequestHandler>::create(config);
    reqHandler->init();

    Json::Value streamConfig;
    streamConfig["name"] = "TestRequestStream";
    streamConfig["type"] = "TestRequestStream";
    streamConfig["requestHandler"] = "TestRequestHandler";
    streamConfig["requests"] = reqs;
    RequestStream* reqStream = Factory<RequestStream>::create(streamConfig);
    reqStream->init();

    simulator::run();

    Factory<RequestHandler>::finish();
    Factory<RequestStream>::finish();
    Factory<PerfModel>::finish();
    Factory<Distribution>::finish();
    ObjectPool<Request>::finish();
    simulator::reset();
}

void addTestRequestStreamConfig(Json::Value& reqs, double arrivalTime, double size, double completionTime)
{
    Json::Value req;
    req["arrivalTime"] = arrivalTime;
    req["size"] = size;
    if (completionTime >= 0) {
        req["completionTime"] = completionTime;
    }
    reqs.append(req);
}
