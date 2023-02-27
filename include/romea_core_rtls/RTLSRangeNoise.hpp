// Copyright 2022 INRAE, French National Research Institute for Agriculture, Food and Environment
// Add license

#ifndef ROMEA_CORE_RTLS__RTLSRANGENOISE_HPP_
#define ROMEA_CORE_RTLS__RTLSRANGENOISE_HPP_

namespace romea
{

class RTLSRangeNoise
{
public:
  RTLSRangeNoise();

  RTLSRangeNoise(
    const double & noiseA,
    const double & noiseB);

  double computeStd(const double & range);

private:
  double a_;
  double b_;
};

}  // namespace romea

#endif  // ROMEA_CORE_RTLS__RTLSRANGENOISE_HPP_
