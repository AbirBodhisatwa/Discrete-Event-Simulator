#include "request_handler_fb_queue.h"
#include "request.h"
#include "listener.h"
#include "factory.h"

REGISTER_CLASS(RequestHandler, RequestHandlerFbQueue)

//TODO fill in
RequestHandlerFbQueue :: RequestHandlerFbQueue(Json :: Value& config) : RequestHandler(config)
{
   _perfModel = Factory<PerfModel>::create(config["perfModel"]);
   start_last_job = 0;
   num_req = 0;
   u_time = 0;
}

RequestHandlerFbQueue :: ~RequestHandlerFbQueue()
{

}

void RequestHandlerFbQueue :: handleRequest(Request* req, ListenerEnd<Request*>* completionCallback)
{
   //update the remaining work for every request in queue
   UpdateQueue(); 
   
   num_req++;
   req->setRemainingWork(_perfModel);
   
   queue_request.insert(std :: pair<uint64_t,Request*>(req->getRemainingWork(),req));
   attained_work.insert(std :: pair<uint64_t,Request*>(0,req));


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
     uint64_t min_size = it->first;
     simulator :: removeEvent(c_event);
    
     uint64_t extra_work = get_work_less_than_minwork(min_size);
     c_time = extra_work;

     c_event = addCompletionEvent(simulator :: getSimTime() + c_time, it->second);
     c_req = it->second;    
     start_last_job = simulator :: getSimTime();
   }

   _requestCompletionCallbacks[req] = completionCallback;
}

void RequestHandlerFbQueue :: notifyEnd(Request* req)
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
      
      uint64_t extra_work = get_work_less_than_minwork(next_req->getRemainingWork());  //extra work needed to bring every req to same level

      next_completion_time = simulator :: getSimTime() + extra_work;

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

unsigned int RequestHandlerFbQueue :: getQueueLength()
{
    return static_cast<unsigned int>(queue_request.size());
}

uint64_t RequestHandlerFbQueue :: getRemainingWorkLeft()
{
    uint64_t totalWork = request_size_extractor();
    uint64_t workDone = simulator :: getSimTime() - start_last_job; 
    uint64_t remaining_work = totalWork - workDone;
    if (remaining_work > 0)
       return remaining_work;
    return 0;
}

uint64_t RequestHandlerFbQueue :: request_size_extractor()
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

void RequestHandlerFbQueue :: UpdateQueue()
{
    std :: multimap<uint64_t,Request*> temp_queue;
    std :: multimap <uint64_t,Request*>::iterator it;
    
    if (num_req != 0)
    {
       attained_work.insert(std :: pair<uint64_t,Request*>((simulator :: getSimTime() - start_last_job)/num_req , c_req));  
       uint64_t running_time = (simulator :: getSimTime() - u_time);
       update_attained_work(running_time);
     }
     u_time = simulator :: getSimTime();
}

uint64_t RequestHandlerFbQueue :: get_attained_service(Request* req)
{
   std :: multimap <uint64_t,Request*>::iterator it;
   for(it =  attained_work.begin(); it !=  attained_work.end(); it++)
   {
        if(it->second  == req)
         {
           return it->first;
         }       
   }
   return 0;
}

void RequestHandlerFbQueue :: update_attained_work(uint64_t running_time)
{
   std :: multimap <uint64_t,Request*>::iterator it;
   uint64_t i = 0; 
       for(it = attained_work.begin(); it != attained_work.end(); it++)
        {
          if(i > 0)
          { 
             uint64_t new_work = (find(i) - find(i-1))*i;
             if (new_work <= running_time)
             {
               running_time = running_time - new_work;
               Request* r = it->second;
               r->setRemainingWork(r->getRemainingWork() - new_work);
             }
             else
              break;
          }
        }
}

uint64_t RequestHandlerFbQueue :: get_work_less_than_minwork(uint64_t min_work)
{
   std :: multimap <uint64_t,Request*>::iterator it;
   uint64_t work = 0;
   for(it = queue_request.begin(); it != queue_request.end(); it++)
   {
       Request* R = it->second;
       if (get_attained_service(R) < min_work)
       {
          work += min_work - get_attained_service(R);
       }
   }
   return work;
}

uint64_t RequestHandlerFbQueue :: find(uint64_t index)
{
   std :: multimap <uint64_t,Request*>::iterator it;
   uint64_t i = 0, w = 0;

   for(it = attained_work.begin(); it != attained_work.end(); it++)
   {
     if(i == index)
     {
       w = it->first;
       break;
     }
   }
 
   return w; 
}

