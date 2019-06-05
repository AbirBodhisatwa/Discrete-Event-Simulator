#ifndef STATS_LATENCY_H
#define STATS_LATENCY_H

#include "stats.h"
#include "request.h"
#include "json/json.h"

// Track latency statistics
class StatsLatency : public Stats
{
private:
    //TODO fill in
    int _numcompletions;
    uint64_t total_time;
    // Functions for outputting statistics
    // Stats classes should override these functions for outputting stats
    virtual void printStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd);
    virtual Json::Value jsonStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd);

public:
    StatsLatency(Json::Value& config);
    virtual ~StatsLatency();
    // Calculate mean latency in seconds
    double getMeanLatency();
    // Reset stats for current interval
    // Stats classes should override this function if there is state to reset every interval
    virtual void resetStats();
    // Called when a request is completed by a target
    virtual void notifyEnd(Request* req);
};

#endif /* STATS_LATENCY_H */
