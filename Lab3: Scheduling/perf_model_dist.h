#ifndef PERF_MODEL_DIST_H
#define PERF_MODEL_DIST_H

#include "json/json.h"
#include "request.h"
#include "perf_model.h"
#include "distribution.h"

class PerfModelDist : public PerfModel
{
private:
    //TODO fill in
    Distribution* obj;

public:
    PerfModelDist(Json::Value& config);
    virtual ~PerfModelDist();
    // Estimates the amount of work to perform a request in nanoseconds
    virtual uint64_t estimateWork(Request* req);
};

#endif /* PERF_MODEL_DIST_H */
