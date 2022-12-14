#include "romea_core_rtls/RTLSRangeRandomNoise.hpp"

namespace romea {

//-----------------------------------------------------------------------------
RTLSRangeRandomNoise::RTLSRangeRandomNoise():
  RTLSRangeNoise(0, 0),
  generator_(std::random_device{}()),
  dist_(0, 1)
{
}

//-----------------------------------------------------------------------------
RTLSRangeRandomNoise::RTLSRangeRandomNoise(const double & a,
                                           const double & b):
  RTLSRangeNoise(a, b)
{
}

//-----------------------------------------------------------------------------
double RTLSRangeRandomNoise::draw(const double & range)
{
  return dist_(generator_)*computeStd(range);
}

}  // namespace romea
