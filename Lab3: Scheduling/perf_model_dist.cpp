#include "perf_model_dist.h"
#include "request.h"
#include "time_helper.h"
#include "factory.h"

REGISTER_CLASS(PerfModel, PerfModelDist)

//TODO fill in

PerfModelDist :: PerfModelDist(Json::Value& config):PerfModel(config) 
{
  obj = Factory<Distribution>::create(config["dist"]);
}

PerfModelDist :: ~PerfModelDist()
{

}

uint64_t PerfModelDist :: estimateWork(Request *req)
{
   return convertSecondsToTime(obj->nextRand());
}

