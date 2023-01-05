// Copyright 2022 INRAE, French National Research Institute for Agriculture, Food and Environment
// Add license

#include "romea_core_rtls/RTLSRangeNoise.hpp"

namespace romea
{


//-----------------------------------------------------------------------------
RTLSRangeNoise::RTLSRangeNoise()
: a_(0),
  b_(0)
{
}

//-----------------------------------------------------------------------------
RTLSRangeNoise::RTLSRangeNoise(
  const double & noiseA,
  const double & noiseB)
: a_(noiseA),
  b_(noiseB)
{
}

//-----------------------------------------------------------------------------
double RTLSRangeNoise::computeStd(const double & range)
{
  return a_ * range + b_;
}

}  // namespace romea
