#ifndef DISTRIBUTION_SUM_H
#define DISTRIBUTION_SUM_H

#include "json/json.h"
#include "distribution.h"


class DistributionSum : public Distribution
{
private:
    //TODO fill in
    std::vector<Distribution*> Dist; 

public:
    DistributionSum(Json::Value& config);
    virtual ~DistributionSum();
    // Returns next random number
    virtual double nextRand();
};

#endif /* DISTRIBUTION_SUM_H */
