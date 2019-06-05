#include "distribution_sum.h"
#include "factory.h"

REGISTER_CLASS(Distribution, DistributionSum)

//TODO fill in
DistributionSum :: DistributionSum(Json::Value& config): Distribution(config)
{
   int i = 0;
   while (i < (int)config["dists"].size())
   {
    Dist.push_back(Factory<Distribution>::create(config["dists"][i]));
    i++;
   }
}

DistributionSum :: ~DistributionSum()
{

}

double DistributionSum :: nextRand()
{
   double sum = 0;
   int i = 0;
   while (i < (int)Dist.size())
   {
     sum += Dist[i]->nextRand();
     i++;
   }
   return sum;
}

