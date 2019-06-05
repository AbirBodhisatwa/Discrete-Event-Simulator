#include "request_handler_central_queue_k.h"
#include "request.h"
#include "listener.h"
#include "factory.h"

REGISTER_CLASS(RequestHandler, RequestHandlerCentralQueueK)

//TODO fill in

RequestHandlerCentralQueueK :: RequestHandlerCentralQueueK(Json :: Value& config) : RequestHandler(config)
{
   _perfModel = Factory<PerfModel>::create(config["perfModel"]);
   k = config["k"].asInt();
   start_last_job = 0;
}

RequestHandlerCentralQueueK :: ~RequestHandlerCentralQueueK()
{

}

void RequestHandlerCentralQueueK :: handleRequest(Request* req, ListenerEnd<Request*>* completionCallback)
{
  
  req->setRemainingWork(_perfModel);
  queue_request.push_back(req);

  if ((int)queue_request.size() <= k)
   {
     uint64_t completionTime = simulator::getSimTime() + req->getRemainingWork();
     addCompletionEvent(completionTime, req);
     start_last_job = simulator :: getSimTime();
   }
   _requestCompletionCallbacks[req] = completionCallback;
}

void RequestHandlerCentralQueueK :: notifyEnd(Request* req)
{
    RemoveReq(req);
     
    if ((int)queue_request.size() >= k)
    {
      Request* next_req;
      uint64_t next_completion_time;
      next_req  = queue_request[k-1];
      next_completion_time = simulator :: getSimTime() + next_req->getRemainingWork();
      addCompletionEvent(next_completion_time, next_req);
      start_last_job = simulator :: getSimTime();
    }

     ListenerEnd<Request*>* completionCallback = _requestCompletionCallbacks[req];
    _requestCompletionCallbacks.erase(req);
    notifyListenersEnd(req);
    completionCallback->notifyEnd(req);
}

unsigned int RequestHandlerCentralQueueK :: getQueueLength()
{
    return static_cast<unsigned int>(queue_request.size());
}

uint64_t RequestHandlerCentralQueueK :: getRemainingWorkLeft()
{
    uint64_t totalWork = request_size_extractor(queue_request);
    uint64_t workDone = simulator :: getSimTime() - start_last_job; 
    uint64_t remaining_work = totalWork - workDone;
    if (remaining_work > 0)
       return remaining_work;
    return 0;
}

uint64_t RequestHandlerCentralQueueK :: request_size_extractor(std :: deque<Request*> temp_queue)
{
      Request* R;
      uint64_t sizes = 0;
      while(!temp_queue.empty())
      {
          R = *(temp_queue.begin());
          temp_queue.erase(temp_queue.begin());
          sizes += R->getRemainingWork();     
      }
     return sizes;
}

void RequestHandlerCentralQueueK :: RemoveReq(Request* req)
{
     //remove the serviced request 
     for (std::deque<Request*>::iterator it = queue_request.begin(); it != queue_request.end(); ++it)
      {
         if (*(it) == req)
          {
            queue_request.erase(it);
            break;
          }
      }
}
