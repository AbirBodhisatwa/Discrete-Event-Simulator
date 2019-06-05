#ifndef TEST_REQUEST_HANDLER_H
#define TEST_REQUEST_HANDLER_H

#include <unordered_map>
#include "../request_handler.h"
#include "../request.h"
#include "../perf_model.h"
#include "../listener.h"
#include "../json/json.h"

class TestRequestHandler : public RequestHandler
{
private:
    std::unordered_map<Request*, ListenerEnd<Request*>*> _requestCompletionCallbacks;
    std::unordered_map<Request*, uint64_t> _completionTimes;
    PerfModel* _perfModel;
    unsigned int _numArrivals;
    unsigned int _numCompletions;

public:
    TestRequestHandler(Json::Value& config);
    virtual ~TestRequestHandler();

    unsigned int numArrivals()
    {
        return _numArrivals;
    }

    unsigned int numCompletions()
    {
        return _numCompletions;
    }

    // Handle request sent to server
    virtual void handleRequest(Request* req, ListenerEnd<Request*>* completionCallback);

    // Called when request is complete
    virtual void notifyEnd(Request* req);

    // Get current queue length
    virtual unsigned int getQueueLength();

    // Get remaining work left in queue in nanoseconds
    virtual uint64_t getRemainingWorkLeft();
};

class CompletionRequestDestroy : public ListenerEnd<Request*>
{
public:
    CompletionRequestDestroy()
    {
    }
    ~CompletionRequestDestroy()
    {
    }
    // Called when request is completed at request handler
    virtual void notifyEnd(Request* req)
    {
        req->destroy();
    }
};

TestRequestHandler* createTestRequestHandler(std::string name, double serviceRate);

#endif /* TEST_REQUEST_HANDLER_H */
