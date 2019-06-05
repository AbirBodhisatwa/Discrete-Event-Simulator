#include "arrival_process_dist.h"
#include "distribution.h"
#include "time_helper.h"
#include "factory.h"

REGISTER_CLASS(ArrivalProcess, ArrivalProcessDist)

//TODO fill in
ArrivalProcessDist ::  ArrivalProcessDist(Json::Value& config) : ArrivalProcess(config)
{
    _obj = Factory<Distribution>::create(config["dist"]);
    //_name = (config["dist"]["name"]).asCString();
    //_type =  (config["dist"]["type"]).asCString();
    //_value = (config["dist"]["val"]).asDouble();
}

ArrivalProcessDist :: ~ArrivalProcessDist()
{

}

uint64_t ArrivalProcessDist :: nextArrival(uint64_t currentTime)
{ 
  
        double num = _obj->nextRand();  
        return(currentTime + convertSecondsToTime(num));  
}
