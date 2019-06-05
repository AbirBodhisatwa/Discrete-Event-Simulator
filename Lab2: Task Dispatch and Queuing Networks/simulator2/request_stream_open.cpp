#include "request_stream_open.h"
#include "factory.h"

REGISTER_CLASS(RequestStream, RequestStreamOpen)

//TODO fill in
RequestStreamOpen::RequestStreamOpen(Json::Value& config): RequestStream(config)
{
  _arrival = Factory<ArrivalProcess>::create(config["arrivalProcessConfig"]);
  _req_gen = Factory<RequestGenerator>::create(config["requestGeneratorConfig"]);
  _initial_time = simulator::getSimTime();
}

RequestStreamOpen :: ~RequestStreamOpen()
{

}

void RequestStreamOpen :: init()
{
   addArrivalEvent();
} 

Request* RequestStreamOpen :: next()
{
   uint64_t arrival_time = _arrival->nextArrival(_initial_time);
   Request* request = _req_gen->nextRequest(arrival_time);
   _initial_time = arrival_time;
   return request;
}

void RequestStreamOpen :: notifyStart(Request* request)
{
     RequestStream :: notifyStart(request);
     addArrivalEvent();
}
