#ifndef STATS_THROUGHPUT_H
#define STATS_THROUGHPUT_H

#include "stats.h"
#include "request.h"
#include "json/json.h"

// Track throughput statistics
class StatsThroughput : public Stats
{
private:
    //TODO fill in
    int _numcompletions;
    // Functions for outputting statistics
    // Stats classes should override these functions for outputting stats
    virtual void printStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd);
    virtual Json::Value jsonStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd);

public:
    StatsThroughput(Json::Value& config);
    virtual ~StatsThroughput();
    // Calculate throughput in reqs/sec
    double getThroughput(uint64_t intervalTime);
    // Reset stats for current interval
    // Stats classes should override this function if there is state to reset every interval
    virtual void resetStats();
    // Called when a request is completed by a target
    virtual void notifyEnd(Request* req);
};

#endif /* STATS_THROUGHPUT_H */
