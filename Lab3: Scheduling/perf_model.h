#ifndef PERF_MODEL_H
#define PERF_MODEL_H

#include "json/json.h"

class Request;

class PerfModel
{
public:
    PerfModel(Json::Value& config)
    {
    }
    virtual ~PerfModel()
    {
    }
    // Estimates the amount of work to perform a request in nanoseconds
    virtual uint64_t estimateWork(Request* req) = 0;
};

#endif /* PERF_MODEL_H */
