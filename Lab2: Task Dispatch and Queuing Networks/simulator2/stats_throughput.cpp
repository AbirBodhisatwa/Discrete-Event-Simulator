#include <iostream>
#include "stats_throughput.h"
#include "time_helper.h"
#include "factory.h"

REGISTER_CLASS(Stats, StatsThroughput)

//TODO fill in

StatsThroughput :: StatsThroughput(Json :: Value& config):Stats(config)
{
       _numcompletions = 0;      
}

StatsThroughput :: ~StatsThroughput()
{

}

void StatsThroughput::printStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd)
{
    std::cout << "t " << lastIntervalBegin
              << " X " << getThroughput(lastIntervalEnd - lastIntervalBegin)
              << std::endl;
}

Json::Value StatsThroughput::jsonStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd)
{
    Json::Value results;
    results["t"] = lastIntervalBegin;
    results["X"] = getThroughput(lastIntervalEnd - lastIntervalBegin);
    return results;
}

double StatsThroughput :: getThroughput (uint64_t intervalTime)
{
    if (intervalTime == 0)
        return 0.0;

    double time = convertTimeToSeconds(intervalTime);
    double throughput = (double) _numcompletions/time; 
    return throughput;   
}

void StatsThroughput :: notifyEnd(Request* req)
{
  
  if(checkOutputStats())
  {
     resetStats();     
  }
  _numcompletions++;
}

void StatsThroughput :: resetStats()
{
   _numcompletions = 0;
}
