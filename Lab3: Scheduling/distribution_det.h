#ifndef DISTRIBUTION_DET_H
#define DISTRIBUTION_DET_H

#include "json/json.h"
#include "distribution.h"

class DistributionDet : public Distribution
{
private:
    //TODO fill in
    double _value; 

public:
    DistributionDet(Json::Value& config);
    virtual ~DistributionDet();
    // Returns next random number
    virtual double nextRand();
};

#endif /* DISTRIBUTION_DET_H */
