#ifndef REQUEST_GENERATOR_H
#define REQUEST_GENERATOR_H

#include <cstdint>
#include "request.h"
#include "json/json.h"

class RequestGenerator
{
public:
    RequestGenerator(Json::Value& config)
    {
    }
    virtual ~RequestGenerator()
    {
    }
    // Returns next request
    virtual Request* nextRequest(uint64_t arrivalTime) = 0;
};

#endif /* REQUEST_GENERATOR_H */
