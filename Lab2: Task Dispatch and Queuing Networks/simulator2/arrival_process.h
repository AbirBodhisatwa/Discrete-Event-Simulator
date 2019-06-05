#ifndef ARRIVAL_PROCESS_H
#define ARRIVAL_PROCESS_H

#include <cstdint>
#include "json/json.h"

class ArrivalProcess
{
public:
    ArrivalProcess(Json::Value& config)
    {
    }
    virtual ~ArrivalProcess()
    {
    }
    // Returns next arrival time (nanoseconds)
    virtual uint64_t nextArrival(uint64_t currentTime) = 0;
};

#endif /* ARRIVAL_PROCESS_H */
