#ifndef REQUEST_HANDLER_RANDOM_QUEUE_H
#define REQUEST_HANDLER_RANDOM_QUEUE_H

#include "request_handler.h"
#include "request.h"
#include "listener.h"
#include "json/json.h"
#include "perf_model.h"

#include <queue>
#include <unordered_map>

class RequestHandlerRandomQueue : public RequestHandler
{
private:
    //TODO fill in
    std :: queue<Request*> queue_request;
    uint64_t start_last_job;
    unsigned int request_index;
    std :: unordered_map<Request*, ListenerEnd<Request*>*> _requestCompletionCallbacks;
    PerfModel* _perfModel;

public:
    RequestHandlerRandomQueue(Json::Value& config);
    virtual ~RequestHandlerRandomQueue();

    // Handle request sent to server
    virtual void handleRequest(Request* req, ListenerEnd<Request*>* completionCallback);

    // Called when request is complete
    virtual void notifyEnd(Request* req);

    // Get current queue length
    virtual unsigned int getQueueLength();

    // Get remaining work left in queue in nanoseconds
    virtual uint64_t getRemainingWorkLeft();

    //helper function for calculating remaining work
    uint64_t request_size_extractor(std :: queue<Request*> temp_queue);

    //helper function to return random request from the queue
    Request* get_Random_Req(std :: queue<Request*> temp_queue);

    //helper function to remove that random request from queue
    void RemoveReq();
};

#endif /* REQUEST_HANDLER_RANDOM_QUEUE_H */
