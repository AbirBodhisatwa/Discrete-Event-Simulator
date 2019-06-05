#include "request_generator_dist.h"
#include "distribution.h"
#include "factory.h"

REGISTER_CLASS(RequestGenerator, RequestGeneratorDist)

//TODO fill in
RequestGeneratorDist :: RequestGeneratorDist(Json::Value& config) : RequestGenerator(config)
{
   _obj = Factory<Distribution>::create(config["dist"]);
   //_name = (config["dist"]["name"]).asCString();
   //_type =  (config["dist"]["type"]).asCString();
   //_value = (config["dist"]["val"]).asDouble();
}

RequestGeneratorDist :: ~RequestGeneratorDist()
{
   
}

Request* RequestGeneratorDist :: nextRequest(uint64_t arrivalTime)
{
   double num = _obj->nextRand();
   uint64_t requestSize = (uint64_t) num;

   Request* next_req = Request::create(arrivalTime, requestSize);
   
   return next_req;
}
