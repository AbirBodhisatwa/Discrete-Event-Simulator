#include "perf_model_service_rate.h"
#include "request.h"
#include "time_helper.h"
#include "factory.h"

REGISTER_CLASS(PerfModel, PerfModelServiceRate)

//TODO fill in
PerfModelServiceRate :: PerfModelServiceRate(Json::Value& config): PerfModel(config)
{
     _speed = config["serviceRate"].asDouble();
}

PerfModelServiceRate :: ~PerfModelServiceRate()
{

}

uint64_t PerfModelServiceRate :: estimateWork(Request* req)
{
   double size = (double)req -> getSize();
   
   return convertSecondsToTime(size/ _speed); 
}
