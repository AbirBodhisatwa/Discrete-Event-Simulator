#include "request_handler_ps_queue.h"
#include "request.h"
#include "listener.h"
#include "factory.h"

REGISTER_CLASS(RequestHandler, RequestHandlerPsQueue)

//TODO fill in

RequestHandlerPsQueue :: RequestHandlerPsQueue(Json :: Value& config) : RequestHandler(config)
{
   _perfModel = Factory<PerfModel>::create(config["perfModel"]);
   start_last_job = 0;
   num_req = 0;
   u_time = 0;
}

RequestHandlerPsQueue :: ~RequestHandlerPsQueue()
{

}

void RequestHandlerPsQueue :: handleRequest(Request* req, ListenerEnd<Request*>* completionCallback)
{
   //update the remaining work for every queue
   UpdateQueue(); 
   num_req++;
   req->setRemainingWork(_perfModel);
   
    queue_request.insert(std :: pair<uint64_t,Request*>(req->getRemainingWork(),req));

    if (queue_request.size() == 1)
   {
     c_req = req;
     uint64_t completionTime = simulator::getSimTime() + req->getRemainingWork();
     c_event = addCompletionEvent(completionTime, req);
     c_time = completionTime;
     start_last_job = simulator :: getSimTime();
   }

   //update the time for the next completion
   else
   {
     std :: multimap <uint64_t,Request*>::iterator it;
     it = queue_request.begin();
     uint64_t min_work = it->first;
     
    simulator::removeEvent(c_event);
    c_time = min_work*num_req;
    c_event = addCompletionEvent(simulator:: getSimTime() + c_time, it->second);
    c_req = it->second;    
    start_last_job = simulator :: getSimTime(); 
   }

   _requestCompletionCallbacks[req] = completionCallback;
}

void RequestHandlerPsQueue :: notifyEnd(Request* req)
{
     UpdateQueue();

    //remove the request from the queue
    std :: multimap <uint64_t,Request*>::iterator it;
    for(it = queue_request.begin(); it != queue_request.end(); it++)
    {
      if(it->second == req)
        {
          queue_request.erase(it);
          num_req--;
          break;
        }   
    }

    if (queue_request.size() > 0)
    {
      Request* next_req;
      uint64_t next_completion_time;
      it = queue_request.begin();
      next_req  = it->second;

      next_completion_time = simulator :: getSimTime() + num_req*(next_req->getRemainingWork());
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

unsigned int RequestHandlerPsQueue :: getQueueLength()
{
    return static_cast<unsigned int>(queue_request.size());
}

uint64_t RequestHandlerPsQueue :: getRemainingWorkLeft()
{
    uint64_t totalWork = request_size_extractor();
    uint64_t workDone = simulator :: getSimTime() - start_last_job; 
    uint64_t remaining_work = totalWork - workDone;
    if (remaining_work > 0)
       return remaining_work;
    return 0;
}

uint64_t RequestHandlerPsQueue :: request_size_extractor()
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

void RequestHandlerPsQueue :: UpdateQueue()
{
    std :: multimap<uint64_t,Request*> temp_queue;
    std :: multimap <uint64_t,Request*>::iterator it;
   
    if (num_req != 0)
    {
       uint64_t running_time = (simulator :: getSimTime() - u_time)/num_req;

       for(it = queue_request.begin(); it != queue_request.end(); it++)
        {
          Request* req = it->second;
          uint64_t work_left = req->getRemainingWork() - running_time; 
          req->setRemainingWork(work_left); 
          temp_queue.insert(std :: pair<uint64_t,Request*>(req->getRemainingWork(),req)); 
          queue_request.erase(it);  
        }

       for(it = temp_queue.begin(); it != temp_queue.end(); it++)
       {
          Request* req = it->second;
          queue_request.insert(std :: pair<uint64_t,Request*>(req->getRemainingWork(),req));   
          temp_queue.erase(it);
       }
     }
     u_time = simulator :: getSimTime();

}
