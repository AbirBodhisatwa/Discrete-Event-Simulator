#include "request_stream_closed.h"
#include "factory.h"
#include "time_helper.h"

REGISTER_CLASS(RequestStream, RequestStreamClosed)

//TODO fill in

RequestStreamClosed :: RequestStreamClosed(Json::Value& config) : RequestStream(config)
{
  _req_gen = Factory<RequestGenerator>::create(config["requestGeneratorConfig"]);
  _think_dist_obj = Factory<Distribution>::create(config["thinkTime"]);

  //think_dist_name = config["thinkTime"]["name"].asCString();
  //think_dist_type = config["thinkTime"]["type"].asCString();
  //think_dist_value = config["thinkTime"]["val"].asDouble();

  MPL = config["MPL"].asInt();

  _initial_time = simulator::getSimTime(); 
} 

RequestStreamClosed :: ~RequestStreamClosed()
{

}

void RequestStreamClosed :: init()
{
   int n = 0;
   while(n < MPL)
   {
     addArrivalEvent();
     n++;
   }
}

Request* RequestStreamClosed :: next()
{
   uint64_t think_time = convertSecondsToTime(_think_dist_obj->nextRand());
   uint64_t arrival_time = simulator::getSimTime() + think_time;

   Request* request = _req_gen->nextRequest(arrival_time);
   return request;
}

void RequestStreamClosed :: notifyEnd(Request* R)
{
   R->destroy();
   addArrivalEvent();
}
