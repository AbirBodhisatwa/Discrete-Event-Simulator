#ifndef STATS_TAIL_LATENCY_H
#define STATS_TAIL_LATENCY_H

#include "stats.h"
#include "request.h"
#include "json/json.h"
#include <cmath>
#include <vector>


// Track tail latency statistics
class StatsTailLatency : public Stats
{
private:
    std::vector<double> _percentiles; // percentiles to output
    //TODO fill in
    std :: vector<uint64_t> req_latencies;
    // Functions for outputting statistics
    // Stats classes should override these functions for outputting stats
    virtual void printStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd);
    virtual Json::Value jsonStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd);

public:
    StatsTailLatency(Json::Value& config);
    virtual ~StatsTailLatency();
    // Calculate tail latency in seconds
    double getTailLatency(double p);
    // Calculate the index for a tail percentile p
    uint64_t getTailPercentileIndex(double p, uint64_t numEntries)
    {
        return static_cast<uint64_t>(round(p * static_cast<double>(numEntries - 1)));
    }
    // Reset stats for current interval
    // Stats classes should override this function if there is state to reset every interval
    virtual void resetStats();
    // Called when a request is completed by a target
    virtual void notifyEnd(Request* req);
    uint64_t getLatency(std::vector<uint64_t> temp, uint64_t index);

};

#endif /* STATS_TAIL_LATENCY_H */
