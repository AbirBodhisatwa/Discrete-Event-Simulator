#ifndef REQUEST_HANDLER_LCFS_QUEUE_H
#define REQUEST_HANDLER_LCFS_QUEUE_H

#include "request_handler.h"
#include "request.h"
#include "listener.h"
#include "json/json.h"
#include "perf_model.h"

#include <stack>
#include <unordered_map>

class RequestHandlerLcfsQueue : public RequestHandler
{
private:
    //TODO fill in
    std :: stack<Request*> queue_request;
    uint64_t start_last_job;
    std :: unordered_map<Request*, ListenerEnd<Request*>*> _requestCompletionCallbacks;
    PerfModel* _perfModel;

public:
    RequestHandlerLcfsQueue(Json::Value& config);
    virtual ~RequestHandlerLcfsQueue();

    // Handle request sent to server
    virtual void handleRequest(Request* req, ListenerEnd<Request*>* completionCallback);

    // Called when request is complete
    virtual void notifyEnd(Request* req);

    // Get current queue length
    virtual unsigned int getQueueLength();

    // Get remaining work left in queue in nanoseconds
    virtual uint64_t getRemainingWorkLeft();

    //helper function for calculating remaining work
    uint64_t request_size_extractor(std :: stack<Request*> temp_queue);

    //helper function to remove the serviced request
    void RemoveReq(Request* req);

};

#endif /* REQUEST_HANDLER_LCFS_QUEUE_H */
