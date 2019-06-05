#include "request_handler_dispatcher_round_robin.h"
#include "factory.h"

REGISTER_CLASS(RequestHandler, RequestHandlerDispatcherRoundRobin)

//TODO fill in

RequestHandlerDispatcherRoundRobin :: RequestHandlerDispatcherRoundRobin(Json::Value& config):RequestHandlerDispatcher(config)
{
  current_index = -1; 
}

RequestHandlerDispatcherRoundRobin :: ~RequestHandlerDispatcherRoundRobin()
{

}

unsigned int RequestHandlerDispatcherRoundRobin :: selectRequestHandler(const Request* req, const std::vector<RequestHandler*>& reqHandlers)
{
   int total = (int)reqHandlers.size();
   if (current_index < total-1)
     {
       current_index += 1;
     }
   else
     {
        current_index = 0;
     }
   return (unsigned int)current_index;
}
