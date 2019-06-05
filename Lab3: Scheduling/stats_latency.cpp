#include <iostream>
#include "stats_latency.h"
#include "time_helper.h"
#include "factory.h"

REGISTER_CLASS(Stats, StatsLatency)

//TODO fill in

StatsLatency :: StatsLatency (Json ::Value& config): Stats(config)
{
  _numcompletions = 0;
  total_time = 0;
}

StatsLatency :: ~StatsLatency()
{
  
}

double StatsLatency :: getMeanLatency()
{
   if (_numcompletions == 0)
      return 0.0;

   double meanlatency = convertTimeToSeconds(total_time)/_numcompletions;
   return meanlatency; 
}

void StatsLatency :: notifyEnd(Request* req)
{

  if(checkOutputStats())
    {
       resetStats();     
    }
  _numcompletions++;
  total_time += req->getElapsedTime();
}

void StatsLatency :: resetStats()
{
      _numcompletions = 0; 
      total_time = 0; 
}
void StatsLatency::printStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd)
{
    std::cout << "t " << lastIntervalBegin
              << " E[T] " << getMeanLatency()
              << std::endl;
}

Json::Value StatsLatency::jsonStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd)
{
    Json::Value results;
    results["t"] = lastIntervalBegin;
    results["E[T]"] = getMeanLatency();
    return results;
}
