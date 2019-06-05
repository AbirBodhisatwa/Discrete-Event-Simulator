#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <unordered_set>
#include "request.h"
#include "notifier.h"
#include "listener.h"
#include "event.h"
#include "simulator.h"
#include "json/json.h"

// Base class for a request handler (e.g., queue, server, load balancer, etc)
class RequestHandler : public NotifierEnd<Request*>, public ListenerEnd<Request*>
{
private:
    std::unordered_set<ListenerEnd<Request*>*> _listenersReqEnd;

public:
    RequestHandler(Json::Value& config)
    {
    }
    virtual ~RequestHandler()
    {
    }

    // Initialize request handler
    virtual void init()
    {
    }

    // Handle a request and notify completionCallback when request is complete
    virtual void handleRequest(Request* req, ListenerEnd<Request*>* completionCallback) = 0;

    // Add a request end listener
    virtual void addListenerEnd(ListenerEnd<Request*>* listener)
    {
        _listenersReqEnd.insert(listener);
    }

    // Notify all listeners of a request completion
    void notifyListenersEnd(Request* req)
    {
        for (auto listener : _listenersReqEnd) {
            listener->notifyEnd(req);
        }
    }

    // Get current queue length
    virtual unsigned int getQueueLength()
    {
        return 0;
    }

    // Get remaining work left in queue in nanoseconds
    virtual uint64_t getRemainingWorkLeft()
    {
        return 0;
    }

    // Add a completion event to the event queue at the given time for a given request
    simulator::EventReference addCompletionEvent(uint64_t timestamp, Request* req);
};

// Completion callback function
void completionCallback(const Event& e);

#endif /* REQUEST_HANDLER_H */
