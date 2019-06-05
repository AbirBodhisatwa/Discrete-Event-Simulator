#include "distribution_exp.h"
#include "random_helper.h"
#include "factory.h"

#include <math.h>
REGISTER_CLASS(Distribution, DistributionExp)

//TODO fill in
// IMPORTANT: you may not use std::exponential_distribution or any other library for generating an exponential distribution
// Instead, use the uniform01 function in random_helper.h

DistributionExp::DistributionExp(Json::Value& config): Distribution(config)
{
 _rate = config["rate"].asDouble();
}

DistributionExp::~DistributionExp()
{
}

double DistributionExp:: nextRand()
{
    double u, y;
    u = uniform01();
    y = (-1 / _rate)* log(1 - u);
    return y;
}


