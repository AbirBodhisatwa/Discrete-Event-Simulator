#include "distribution_mix.h"
#include "random_helper.h"
#include "factory.h"

#include <stdlib.h>
#include <iterator>
REGISTER_CLASS(Distribution, DistributionMix)

//TODO fill in
DistributionMix::DistributionMix(Json::Value& config): Distribution(config)
{
   int i, total = (int)config["dists"].size(); 
  
   for (i = 0; i < total; i++)
 {
  Distribution* obj = Factory<Distribution>::create(config["dists"][i]);
  dis.push_back(obj);
  dis_prob.insert(std::pair <Distribution*,double> (obj,config["probabilities"][i].asDouble()));  }
}

DistributionMix :: ~DistributionMix()
{

}

double DistributionMix :: nextRand()
{
  double p = uniform01();
  double cp = 0.0, r = 0.0;

  for (std::vector<Distribution*>::iterator it = dis.begin() ; it != dis.end(); ++it)
  {
    cp += dis_prob[*it];
    if (cp >= p)
    {
      r = (*it)->nextRand();
      return r;
    }
  }
  return r;
 }

