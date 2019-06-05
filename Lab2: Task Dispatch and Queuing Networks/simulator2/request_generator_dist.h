#ifndef REQUEST_GENERATOR_DIST_H
#define REQUEST_GENERATOR_DIST_H

#include <cstdint>
#include "json/json.h"
#include "request_generator.h"
#include "distribution.h"

// Request generator for i.i.d. request sizes (in bytes) following a given distribution
class RequestGeneratorDist : public RequestGenerator
{
private:
    //TODO fill in
    Distribution* _obj;
    //const char* _name;
    //const char* _type;
    //double _value;

public:
    RequestGeneratorDist(Json::Value& config);
    virtual ~RequestGeneratorDist();
    // Returns next request
    virtual Request* nextRequest(uint64_t arrivalTime);
};

#endif /* REQUEST_GENERATOR_DIST_H */
