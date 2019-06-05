#ifndef REQUEST_STREAM_H
#define REQUEST_STREAM_H

#include <cassert>
#include "request.h"
#include "request_handler.h"
#include "listener.h"
#include "event.h"
#include "factory.h"

// Base class for a stream of requests
class RequestStream : public ListenerStart<Request*>, public ListenerEnd<Request*>
{
private:
    RequestHandler* _requestHandler;

public:
    RequestStream(Json::Value& config)
        : _requestHandler(Factory<RequestHandler>::create(config["requestHandler"]))
    {
    }
    virtual ~RequestStream()
    {
    }

    // Initialize request stream
    virtual void init()
    {
    }

    // Next request in stream
    virtual Request* next() = 0;

    // Called when request arrives
    virtual void notifyStart(Request* req)
    {
        // Send request to handler
        _requestHandler->handleRequest(req, this);
    }

    // Called when request is completed at request handler
    virtual void notifyEnd(Request* req)
    {
        req->destroy();
    }

    // Add an arrival event to the event queue for the next request in reqStream
    void addArrivalEvent();
};

// Arrival callback function
void arrivalCallback(const Event& e);

#endif /* REQUEST_STREAM_H */
