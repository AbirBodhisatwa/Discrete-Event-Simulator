#ifndef REQUEST_HANDLER_SRPT_QUEUE_H
#define REQUEST_HANDLER_SRPT_QUEUE_H

#include "request_handler.h"
#include "request.h"
#include "listener.h"
#include "json/json.h"
#include "perf_model.h"
#include "simulator.h"

#include <map>
#include <unordered_map>

class RequestHandlerSrptQueue : public RequestHandler
{
private:
    //TODO fill in
    std :: multimap<uint64_t,Request*> queue_request;
    uint64_t start_last_job,c_time,num_req;
    simulator :: EventReference c_event;
    std :: unordered_map<Request*, ListenerEnd<Request*>*> _requestCompletionCallbacks;
    PerfModel* _perfModel;
    Request* c_req;
   
public:
    RequestHandlerSrptQueue(Json::Value& config);
    virtual ~RequestHandlerSrptQueue();

    // Handle request sent to server
    virtual void handleRequest(Request* req, ListenerEnd<Request*>* completionCallback);

    // Called when request is complete
    virtual void notifyEnd(Request* req);

    // Get current queue length
    virtual unsigned int getQueueLength();

    // Get remaining work left in queue in nanoseconds
    virtual uint64_t getRemainingWorkLeft();

    //helper function for calculating remaining work
    uint64_t request_size_extractor();

    //helper function for updating the preempted request with remaining work
    void UpdateReq(Request* req);
};

#endif /* REQUEST_HANDLER_SRPT_QUEUE_H */
