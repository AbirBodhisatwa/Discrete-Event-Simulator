#include "request_handler_dispatcher.h"
#include "request.h"
#include "factory.h"
#include <cassert>

RequestHandlerDispatcher::RequestHandlerDispatcher(Json::Value& config)
    : RequestHandler(config),
      _config(config)
{
}

RequestHandlerDispatcher::~RequestHandlerDispatcher()
{
}

void RequestHandlerDispatcher::init()
{
    Json::Value& reqHandlers = _config["reqHandlers"];
    for (unsigned int i = 0; i < reqHandlers.size(); i++) {
        _reqHandlers.push_back(Factory<RequestHandler>::create(reqHandlers[i]));
    }
}

// Forward request to selected server
void RequestHandlerDispatcher::handleRequest(Request* req, ListenerEnd<Request*>* completionCallback)
{
    unsigned int index = selectRequestHandler(req, _reqHandlers);
    _reqHandlers[index]->handleRequest(req, completionCallback);
}

// Called when request is complete
void RequestHandlerDispatcher::notifyEnd(Request* req)
{
    assert(0); // unused
}
