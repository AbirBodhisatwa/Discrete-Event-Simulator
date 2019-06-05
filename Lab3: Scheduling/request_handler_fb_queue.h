#ifndef REQUEST_HANDLER_FB_QUEUE_H
#define REQUEST_HANDLER_FB_QUEUE_H

#include "request_handler.h"
#include "request.h"
#include "listener.h"
#include "json/json.h"
#include "perf_model.h"
#include "simulator.h"


#include <map>
#include <unordered_map>


class RequestHandlerFbQueue : public RequestHandler
{
private:
    //TODO fill in
    std :: multimap<uint64_t,Request*> queue_request;
    std :: multimap<uint64_t,Request*> attained_work;
    uint64_t start_last_job,c_time,num_req,u_time;
    simulator :: EventReference c_event;
    std :: unordered_map<Request*, ListenerEnd<Request*>*> _requestCompletionCallbacks;
    PerfModel* _perfModel;
    Request* c_req;

public:
    RequestHandlerFbQueue(Json::Value& config);
    virtual ~RequestHandlerFbQueue();

    // Handle request sent to server
    virtual void handleRequest(Request* req, ListenerEnd<Request*>* completionCallback);

    // Called when request is complete
    virtual void notifyEnd(Request* req);

    // Get current queue length
    virtual unsigned int getQueueLength();

    // Get remaining work left in queue in nanoseconds
    virtual uint64_t getRemainingWorkLeft();

    // Update the remaninig work
    void UpdateQueue();
     
    ////helper function for calculating remaining work
    uint64_t request_size_extractor();

    uint64_t find(uint64_t index);

    // Get attained service for a particular request
    uint64_t get_attained_service(Request* req);

    uint64_t get_work_less_than_minwork(uint64_t min_work);

    void update_attained_work(uint64_t running_time);
    
};

#endif /* REQUEST_HANDLER_FB_QUEUE_H */
