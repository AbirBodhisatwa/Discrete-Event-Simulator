#include "test_request_handler.h"
#include "../request.h"
#include "../listener.h"
#include "../factory.h"

REGISTER_CLASS(RequestHandler, TestRequestHandler)

TestRequestHandler::TestRequestHandler(Json::Value& config)
    : RequestHandler(config),
      _perfModel(Factory<PerfModel>::create(config["perfModel"])),
      _numArrivals(0),
      _numCompletions(0)
{
}

TestRequestHandler::~TestRequestHandler()
{
}

// Handle request sent to server
void TestRequestHandler::handleRequest(Request* req, ListenerEnd<Request*>* completionCallback)
{
    _numArrivals++;
    req->setRemainingWork(_perfModel);
    uint64_t completionTime = simulator::getSimTime() + req->getRemainingWork();
    _completionTimes[req] = completionTime;
    _requestCompletionCallbacks[req] = completionCallback;
    addCompletionEvent(completionTime, req);
}

// Called when request is complete
void TestRequestHandler::notifyEnd(Request* req)
{
    _numCompletions++;
    _completionTimes.erase(req);
    ListenerEnd<Request*>* completionCallback = _requestCompletionCallbacks[req];
    _requestCompletionCallbacks.erase(req);
    // Notify end listeners
    notifyListenersEnd(req);
    completionCallback->notifyEnd(req);
}

// Get current queue length
unsigned int TestRequestHandler::getQueueLength()
{
    return static_cast<unsigned int>(_completionTimes.size());
}

// Get remaining work left in queue in nanoseconds
uint64_t TestRequestHandler::getRemainingWorkLeft()
{
    uint64_t remainingWork = 0;
    for (auto it = _completionTimes.begin(); it != _completionTimes.end(); it++) {
        remainingWork += it->second - simulator::getSimTime();
    }
    return remainingWork;
}

TestRequestHandler* createTestRequestHandler(std::string name, double serviceRate)
{
    Json::Value config;
    config["name"] = name;
    config["type"] = "TestRequestHandler";
    config["perfModel"]["name"] = name;
    config["perfModel"]["type"] = "PerfModelServiceRate";
    config["perfModel"]["serviceRate"] = serviceRate;
    RequestHandler* reqHandler = Factory<RequestHandler>::create(config);
    reqHandler->init();
    return dynamic_cast<TestRequestHandler*>(reqHandler);
}
