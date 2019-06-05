#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include "json/json.h"

class Distribution
{
public:
    Distribution(Json::Value& config)
    {
    }
    virtual ~Distribution()
    {
    }
    // Returns next random number
    virtual double nextRand() = 0;
};

#endif /* DISTRIBUTION_H */
