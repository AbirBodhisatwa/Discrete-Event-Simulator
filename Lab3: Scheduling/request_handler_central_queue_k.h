#ifndef REQUEST_HANDLER_CENTRAL_QUEUE_K_H
#define REQUEST_HANDLER_CENTRAL_QUEUE_K_H

#include "request_handler.h"
#include "request.h"
#include "listener.h"
#include "json/json.h"
#include "perf_model.h"

#include <deque>
#include <unordered_map>

class RequestHandlerCentralQueueK : public RequestHandler
{
private:
    //TODO fill in
    std :: deque<Request*> queue_request;
    std :: unordered_map<Request*, ListenerEnd<Request*>*> _requestCompletionCallbacks;
    PerfModel* _perfModel;
    uint64_t start_last_job;
    int k;

public:
    RequestHandlerCentralQueueK(Json::Value& config);
    virtual ~RequestHandlerCentralQueueK();

    // Handle request sent to server
    virtual void handleRequest(Request* req, ListenerEnd<Request*>* completionCallback);

    // Called when request is complete
    virtual void notifyEnd(Request* req);

    // Get current queue length
    virtual unsigned int getQueueLength();

    // Get remaining work left in queue in nanoseconds
    virtual uint64_t getRemainingWorkLeft();
  
    //helper function to remove the serviced request
    void RemoveReq(Request* req);

    uint64_t request_size_extractor(std :: deque<Request*> temp_queue);
};

#endif /* REQUEST_HANDLER_CENTRAL_QUEUE_K_H */
