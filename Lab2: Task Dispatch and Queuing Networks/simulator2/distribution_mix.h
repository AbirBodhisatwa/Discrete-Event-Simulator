#ifndef DISTRIBUTION_MIX_H
#define DISTRIBUTION_MIX_H

#include "json/json.h"
#include "distribution.h"

#include <map>

class DistributionMix : public Distribution
{
private:
    //TODO fill in
  std :: vector <Distribution*> dis;
  std :: map <Distribution*,float> dis_prob;  int total;
public:
    DistributionMix(Json::Value& config);
    virtual ~DistributionMix();
    // Returns next random number
    virtual double nextRand();
};

#endif /* DISTRIBUTION_MIX_H */
