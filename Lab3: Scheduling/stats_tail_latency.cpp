#include <iostream>
#include <string>
#include <algorithm>
#include "stats_tail_latency.h"
#include "time_helper.h"
#include "factory.h"

REGISTER_CLASS(Stats, StatsTailLatency)

StatsTailLatency::StatsTailLatency(Json::Value& config)
    : Stats(config)
{
    Json::Value& percentiles = config["percentiles"];
    for (unsigned int i = 0; i < percentiles.size(); i++) {
        _percentiles.push_back(percentiles[i].asDouble());
    }
    resetStats();
}

StatsTailLatency::~StatsTailLatency()
{
}

// Reset stats for current interval
void StatsTailLatency::resetStats()
{
    //TODO fill in
    req_latencies.clear();
     
}

// Called when a request is completed by a target
void StatsTailLatency::notifyEnd(Request* req)
{
    if (checkOutputStats()) {
        resetStats();
    }

    //TODO fill in
    uint64_t latency = req->getElapsedTime();
    req_latencies.push_back(latency);
   
}

double StatsTailLatency::getTailLatency(double p)
{
    //TODO fill in
    uint64_t total_req = req_latencies.size(),latency,index;
    
    index = this->getTailPercentileIndex(p,total_req);
    
    std::nth_element(req_latencies.begin(),req_latencies.begin() + index,req_latencies.end());
    latency = getLatency(req_latencies,index);

    return (convertTimeToSeconds(latency));
}

uint64_t StatsTailLatency::getLatency(std::vector <uint64_t> temp, uint64_t index)
{
     uint64_t latency = 0,k = 0;
     std :: vector<uint64_t> :: iterator it = temp.begin();
     while(temp.size() > 0)
     {
         it = temp.begin();

         if(index == k)
           {
              latency = *it;
           }
       temp.erase(it);
       k++;
     }
    return latency;
}

void StatsTailLatency::printStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd)
{
    std::cout << "t " << lastIntervalBegin;
    for (double p : _percentiles) {
        std::cout << " " << p << " " << getTailLatency(p);
    }
    std::cout << std::endl;
}

Json::Value StatsTailLatency::jsonStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd)
{
    Json::Value results;
    results["t"] = lastIntervalBegin;
    for (double p : _percentiles) {
        results[std::to_string(p)] = getTailLatency(p);
    }
    return results;
}
