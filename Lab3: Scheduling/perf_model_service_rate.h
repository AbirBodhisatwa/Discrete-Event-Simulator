#ifndef PERF_MODEL_SERVICE_RATE_H
#define PERF_MODEL_SERVICE_RATE_H

#include "json/json.h"
#include "request.h"
#include "perf_model.h"

class PerfModelServiceRate : public PerfModel
{
private:
    //TODO fill in
    double _speed;

public:
    PerfModelServiceRate(Json::Value& config);
    virtual ~PerfModelServiceRate();
    // Estimates the amount of work to perform a request in nanoseconds
    virtual uint64_t estimateWork(Request* req);
};

#endif /* PERF_MODEL_SERVICE_RATE_H */
