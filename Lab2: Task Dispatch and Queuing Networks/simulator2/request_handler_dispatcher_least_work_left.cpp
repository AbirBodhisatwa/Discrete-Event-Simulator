#include "request_handler_dispatcher_least_work_left.h"
#include "factory.h"

REGISTER_CLASS(RequestHandler, RequestHandlerDispatcherLeastWorkLeft)

//TODO fill in

RequestHandlerDispatcherLeastWorkLeft :: RequestHandlerDispatcherLeastWorkLeft(Json::Value& config):RequestHandlerDispatcher(config)
{

}

RequestHandlerDispatcherLeastWorkLeft :: ~RequestHandlerDispatcherLeastWorkLeft()
{

}

unsigned int RequestHandlerDispatcherLeastWorkLeft :: selectRequestHandler(const Request* req, const std::vector<RequestHandler*>& reqHandlers)
{
uint64_t l,min;
int i = 0,j = -1;
min =  (*(reqHandlers.begin()))->getRemainingWorkLeft();
 
for(std::vector<RequestHandler*>::const_iterator it = reqHandlers.begin(); it != reqHandlers.end(); ++it)    
{
    l = (*it)-> getRemainingWorkLeft();
    j++;
    if (l <= min)
    {
      min = l;
      i = j;
    }
}
  return (unsigned int)i; 
}
