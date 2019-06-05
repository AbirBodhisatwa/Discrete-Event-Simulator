#include "request_handler_random_queue.h"
#include "request.h"
#include "listener.h"
#include "random_helper.h"
#include "factory.h"

REGISTER_CLASS(RequestHandler, RequestHandlerRandomQueue)

//TODO fill in
RequestHandlerRandomQueue :: RequestHandlerRandomQueue(Json :: Value& config) : RequestHandler(config)
{
   _perfModel = Factory<PerfModel>::create(config["perfModel"]);
   start_last_job = 0;
   request_index = 0;
}

RequestHandlerRandomQueue :: ~RequestHandlerRandomQueue()
{

}

void RequestHandlerRandomQueue :: handleRequest(Request* req, ListenerEnd<Request*>* completionCallback)
{
  queue_request.push(req);
  req->setRemainingWork(_perfModel);

  if (queue_request.size() == 1)
   {
     uint64_t completionTime = simulator::getSimTime() + req->getRemainingWork();
     addCompletionEvent(completionTime, req);
     start_last_job = simulator :: getSimTime();
   }
   
   _requestCompletionCallbacks[req] = completionCallback;
}

void RequestHandlerRandomQueue :: notifyEnd(Request* req)
{
    RemoveReq();
    if (queue_request.size() > 0)
    {
      Request* next_req;
      uint64_t next_completion_time;
      next_req  = get_Random_Req(queue_request);
      next_completion_time = simulator :: getSimTime() + next_req->getRemainingWork();
      addCompletionEvent(next_completion_time, next_req);
      start_last_job = simulator :: getSimTime();
    }

     ListenerEnd<Request*>* completionCallback = _requestCompletionCallbacks[req];
    _requestCompletionCallbacks.erase(req);
    notifyListenersEnd(req);
    completionCallback->notifyEnd(req);
}

unsigned int RequestHandlerRandomQueue :: getQueueLength()
{
    return static_cast<unsigned int>(queue_request.size());
}

uint64_t RequestHandlerRandomQueue :: getRemainingWorkLeft()
{
    uint64_t totalWork = request_size_extractor(queue_request);
    uint64_t workDone = simulator :: getSimTime() - start_last_job; 
    uint64_t remaining_work = totalWork - workDone;
    if (remaining_work > 0)
       return remaining_work;
    return 0;
}

uint64_t RequestHandlerRandomQueue :: request_size_extractor(std :: queue<Request*> temp_queue)
  {
      Request* R;
      uint64_t sizes = 0;
      while(!temp_queue.empty())
      {
          R = temp_queue.front();
          temp_queue.pop();
          sizes += R->getRemainingWork();     
      }
     return sizes;
  }

Request* RequestHandlerRandomQueue :: get_Random_Req(std :: queue<Request*> temp_queue)
{
  double total = (double)temp_queue.size();
  double p = uniform01();
  double prob = (double)(1.0/total);
  double cp = 0.0;
  int i = 0;
  for (i = 0; i < total; i++)
  {
    cp += prob;
    if(cp >= p)
    {
      request_index =  (unsigned int) i;
      break;
    }
  }

  //iterate to that index in the temporary queue
  unsigned int j = 0;
  while( j != request_index)
  {
     temp_queue.pop();
     j++; 
  }
  return temp_queue.front();
}

void RequestHandlerRandomQueue :: RemoveReq()
{
     //remove the request at index request_index
     unsigned int i = 0;
     std :: queue <Request*> temp_queue;
     while(queue_request.size() > 0)
     {
         Request* r = queue_request.front();
         queue_request.pop();

         if(i != request_index)
           {
              temp_queue.push(r);
           }
     i++;        
     }

     while(temp_queue.size() > 0)
      {
          queue_request.push(temp_queue.front());
          temp_queue.pop();
      }
}
