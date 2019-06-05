#include "request_handler_dispatcher_random.h"
#include "random_helper.h"
#include "factory.h"

REGISTER_CLASS(RequestHandler, RequestHandlerDispatcherRandom)

//TODO fill in
RequestHandlerDispatcherRandom :: RequestHandlerDispatcherRandom(Json::Value& config):RequestHandlerDispatcher(config)
{

}

RequestHandlerDispatcherRandom :: ~RequestHandlerDispatcherRandom()
{

}

unsigned int RequestHandlerDispatcherRandom :: selectRequestHandler(const Request* req,const std::vector<RequestHandler*>& reqHandlers)
{
  double total = (double)reqHandlers.size();
  double prob = (double)(1.0/total);
  double cp = 0.0;
  double p = uniform01();
  int i = 0;
  for (i = 0; i < total; i++)
  {
    cp += prob;
    if(cp >= p)
    {
      return (unsigned int) i;
    }
  }
  return (unsigned int)i;
}


