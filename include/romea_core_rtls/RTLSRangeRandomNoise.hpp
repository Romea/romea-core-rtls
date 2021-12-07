#ifndef _romea_RTLSRangeRandomNoise_hpp_
#define _romea_RTLSRangeRandomNoise_hpp_

//romea
#include "RTLSRangeNoise.hpp"

//std
#include <random>

namespace romea
{

class RTLSRangeRandomNoise : public RTLSRangeNoise
{

public :

  RTLSRangeRandomNoise();

  RTLSRangeRandomNoise(const double & a,
                       const double & b);

public:

  double draw(const double & range);

private:

  std::default_random_engine generator_;
  std::normal_distribution<double> dist_;
};


}

#endif
