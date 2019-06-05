#include "request_handler_dispatcher_join_shortest_queue.h"
#include "factory.h"

REGISTER_CLASS(RequestHandler, RequestHandlerDispatcherJoinShortestQueue)

//TODO fill in
RequestHandlerDispatcherJoinShortestQueue::RequestHandlerDispatcherJoinShortestQueue(Json::Value& config):RequestHandlerDispatcher(config)
{
}

RequestHandlerDispatcherJoinShortestQueue::~RequestHandlerDispatcherJoinShortestQueue()
{
}

unsigned int RequestHandlerDispatcherJoinShortestQueue :: selectRequestHandler(const Request* req, const std::vector<RequestHandler*>& reqHandlers)
{
unsigned int l,min;
int i = 0,j = -1;
min =  (*(reqHandlers.begin()))->getQueueLength();
 
for(std::vector<RequestHandler*>::const_iterator it = reqHandlers.begin(); it != reqHandlers.end(); ++it)    
{
    l = (*it)-> getQueueLength();
    j++;
    if (l <= min)
    {
      min = l;
      i = j;
    }
}
  return (unsigned int)i;
}
