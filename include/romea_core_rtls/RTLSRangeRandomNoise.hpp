#ifndef ROMEA_CORE_RTLS_RTLSRANGERANDOMNOISE_HPP
#define ROMEA_CORE_RTLS_RTLSRANGERANDOMNOISE_HPP

// std
#include <random>

// romea
#include "romea_core_rtls/RTLSRangeNoise.hpp"


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

}  // namespace romea

#endif  // ROMEA_CORE_RTLS_RTLSRANGERANDOMNOISE_HPP
