#ifndef ARRIVAL_PROCESS_DIST_H
#define ARRIVAL_PROCESS_DIST_H

#include <cstdint>
#include "json/json.h"
#include "arrival_process.h"
#include "distribution.h"

// Arrival process with i.i.d. interarrival times following a given distribution
class ArrivalProcessDist : public ArrivalProcess
{
private:
    //TODO fill in
    //const char* _name;
    //const char* _type;
    //double _value;
    Distribution* _obj;

public:
    ArrivalProcessDist(Json::Value& config);
    virtual ~ArrivalProcessDist();
    // Returns next arrival time (nanoseconds)
    virtual uint64_t nextArrival(uint64_t currentTime);
};

#endif /* ARRIVAL_PROCESS_DIST_H */
