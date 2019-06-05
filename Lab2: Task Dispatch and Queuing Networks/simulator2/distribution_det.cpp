#include "distribution_det.h"
#include "factory.h"

REGISTER_CLASS(Distribution, DistributionDet)

//TODO fill in
DistributionDet::DistributionDet(Json::Value& config): Distribution(config)
{
  _value = config["val"].asDouble();
}

DistributionDet::~DistributionDet()
{
}

double DistributionDet :: nextRand()
{
  return _value;
}
