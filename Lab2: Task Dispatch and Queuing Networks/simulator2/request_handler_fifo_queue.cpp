#include "request_handler_fifo_queue.h"
#include "request.h"
#include "listener.h"
#include "factory.h"


REGISTER_CLASS(RequestHandler, RequestHandlerFifoQueue)

//TODO fill in
RequestHandlerFifoQueue :: RequestHandlerFifoQueue(Json::Value& config): RequestHandler(config)
{
   _perfModel = Factory<PerfModel>::create(config["perfModel"]);
   start_last_job = 0;
}

RequestHandlerFifoQueue :: ~RequestHandlerFifoQueue()
{

}

void RequestHandlerFifoQueue :: handleRequest(Request* req, ListenerEnd<Request*>*completionCallback)
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

unsigned int RequestHandlerFifoQueue :: getQueueLength()
{
    return static_cast<unsigned int>(queue_request.size());
}

uint64_t RequestHandlerFifoQueue :: getRemainingWorkLeft()
{
    uint64_t totalWork = request_size_extractor(queue_request);
    uint64_t workDone = simulator :: getSimTime() - start_last_job; 
    uint64_t remaining_work = totalWork - workDone;
    if (remaining_work > 0)
       return remaining_work;
    return 0;
}

uint64_t RequestHandlerFifoQueue :: request_size_extractor(std :: queue<Request*> temp_queue)
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
void RequestHandlerFifoQueue :: notifyEnd(Request* req)

{    
    queue_request.pop();
    if (queue_request.size() > 0)
    {
      Request* next_req;
      uint64_t next_completion_time;
      next_req  = queue_request.front();
      next_completion_time = simulator :: getSimTime() + next_req->getRemainingWork();
      addCompletionEvent(next_completion_time, next_req);
      start_last_job = simulator :: getSimTime();
    }

     ListenerEnd<Request*>* completionCallback = _requestCompletionCallbacks[req];
    _requestCompletionCallbacks.erase(req);
    notifyListenersEnd(req);
    completionCallback->notifyEnd(req);
} 
