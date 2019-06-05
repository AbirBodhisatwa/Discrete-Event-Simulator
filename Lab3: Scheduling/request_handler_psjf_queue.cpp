#include "request_handler_psjf_queue.h"
#include "request.h"
#include "listener.h"
#include "factory.h"

REGISTER_CLASS(RequestHandler, RequestHandlerPsjfQueue)

//TODO fill in
RequestHandlerPsjfQueue :: RequestHandlerPsjfQueue(Json :: Value& config) : RequestHandler(config)
{
   _perfModel = Factory<PerfModel>::create(config["perfModel"]);
   start_last_job = 0;
}

RequestHandlerPsjfQueue :: ~RequestHandlerPsjfQueue()
{

}

void RequestHandlerPsjfQueue :: handleRequest(Request* req, ListenerEnd<Request*>* completionCallback)
{
  queue_request.insert(std :: pair<uint64_t,Request*>(req->getSize(),req));
  req->setRemainingWork(_perfModel);
 
   
  if (queue_request.size() == 1)
   {
     c_req = req;
     uint64_t completionTime = simulator::getSimTime() + req->getRemainingWork();
     c_event = addCompletionEvent(completionTime, req);
     c_time = completionTime;
     start_last_job = simulator :: getSimTime();
   }

   else if( req->getSize() < c_req->getSize() )
  {
    c_req->setRemainingWork(c_time - simulator::getSimTime());
    simulator::removeEvent(c_event);
    c_time = simulator::getSimTime() + req->getRemainingWork();
    c_event = addCompletionEvent(c_time, req);
    c_req = req;
  }
   
   _requestCompletionCallbacks[req] = completionCallback;
}

void RequestHandlerPsjfQueue :: notifyEnd(Request* req)
{
    //remove the request from the queue
    std :: multimap <uint64_t,Request*>::iterator it;
    for(it = queue_request.begin(); it != queue_request.end(); it++)
    {
      if(it->second == req)
        {
          queue_request.erase(it);
        }   
    }

    if (queue_request.size() > 0)
    {
      Request* next_req;
      uint64_t next_completion_time;
      it = queue_request.begin();
      next_req  = it->second;

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

unsigned int RequestHandlerPsjfQueue :: getQueueLength()
{
    return static_cast<unsigned int>(queue_request.size());
}

uint64_t RequestHandlerPsjfQueue :: getRemainingWorkLeft()
{
    uint64_t totalWork = request_size_extractor();
    uint64_t workDone = simulator :: getSimTime() - start_last_job; 
    uint64_t remaining_work = totalWork - workDone;
    if (remaining_work > 0)
       return remaining_work;
    return 0;
}

uint64_t RequestHandlerPsjfQueue :: request_size_extractor()
  {
      Request* R;
      uint64_t sizes = 0;
      std :: multimap <uint64_t,Request*>::iterator it;

      for (it=queue_request.begin(); it!=queue_request.end(); it++)
      {
          R = it->second;
          sizes += R->getRemainingWork();     
      }
     R = it->second;
     sizes += R->getRemainingWork(); 
     
     return sizes;
  }
