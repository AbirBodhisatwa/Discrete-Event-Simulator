#ifndef DISTRIBUTION_EXP_H
#define DISTRIBUTION_EXP_H

#include "json/json.h"
#include "distribution.h"

class DistributionExp : public Distribution
{
private:
    //TODO fill in
    double _rate;

public:
    DistributionExp(Json::Value& config);
    virtual ~DistributionExp();
    // Returns next random number
    virtual double nextRand();
};

#endif /* DISTRIBUTION_EXP_H */
