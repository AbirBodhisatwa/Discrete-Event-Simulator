#include "request_handler_plcfs_queue.h"
#include "request.h"
#include "listener.h"
#include "factory.h"

REGISTER_CLASS(RequestHandler, RequestHandlerPlcfsQueue)

//TODO fill in
RequestHandlerPlcfsQueue :: RequestHandlerPlcfsQueue(Json :: Value& config):RequestHandler(config)
{
   _perfModel = Factory<PerfModel>::create(config["perfModel"]);
   start_last_job = 0;
}

RequestHandlerPlcfsQueue :: ~RequestHandlerPlcfsQueue()
{

}

void RequestHandlerPlcfsQueue :: handleRequest(Request* req, ListenerEnd<Request*>* completionCallback)
{
  queue_request.push(req);
  req->setRemainingWork(_perfModel);

  if (queue_request.size() == 1)
   {
     c_req = req;
     uint64_t completionTime = simulator::getSimTime() + req->getRemainingWork();
     c_event = addCompletionEvent(completionTime, req);
     c_time = completionTime;
     start_last_job = simulator :: getSimTime();
   }
   
   else
   //preempt the running job
  {
    c_req->setRemainingWork(c_time - simulator::getSimTime());
    simulator::removeEvent(c_event);
    c_time = simulator::getSimTime() + req->getRemainingWork();
    c_event = addCompletionEvent(c_time, req);
    c_req = req;
    start_last_job = simulator :: getSimTime();
  }
   
   _requestCompletionCallbacks[req] = completionCallback;
}

void RequestHandlerPlcfsQueue :: notifyEnd(Request* req)
{
    RemoveReq(req);
     
    if (queue_request.size() > 0)
    {
      Request* next_req;
      uint64_t next_completion_time;
      next_req  = queue_request.top();
      next_completion_time = simulator :: getSimTime() + next_req->getRemainingWork();
      c_event = addCompletionEvent(next_completion_time, next_req);
      start_last_job = simulator :: getSimTime();
      c_time = next_completion_time;
      c_req = next_req;
    }

     ListenerEnd<Request*>* completionCallback = _requestCompletionCallbacks[req];
    _requestCompletionCallbacks.erase(req);
    notifyListenersEnd(req);
    completionCallback->notifyEnd(req);  
}

unsigned int RequestHandlerPlcfsQueue :: getQueueLength()
{
  return static_cast<unsigned int>(queue_request.size());
}

uint64_t RequestHandlerPlcfsQueue :: getRemainingWorkLeft()
{
    uint64_t totalWork = request_size_extractor(queue_request);
    uint64_t workDone = simulator :: getSimTime() - start_last_job; 
    uint64_t remaining_work = totalWork - workDone;
    if (remaining_work > 0)
       return remaining_work;
    return 0;
}

uint64_t RequestHandlerPlcfsQueue :: request_size_extractor(std :: stack<Request*> temp_queue)
{
      Request* R;
      uint64_t sizes = 0;
      while(!temp_queue.empty())
      {
          R = temp_queue.top();
          temp_queue.pop();
          sizes += R->getRemainingWork();     
      }
     return sizes;
}

void RequestHandlerPlcfsQueue :: RemoveReq(Request* req)
{
     //remove the serviced request 
     std :: stack <Request*> temp_queue;
     while(queue_request.size() > 0)
     {
         Request* r = queue_request.top();
         queue_request.pop();

         if(r != req)
           {
              temp_queue.push(r);
           }
     }

     while(temp_queue.size() > 0)
      {
          queue_request.push(temp_queue.top());
          temp_queue.pop();
      }
}
