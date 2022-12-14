#ifndef ROMEA_CORE_RTLS_RTLSRANGENOISE_HPP_
#define ROMEA_CORE_RTLS_RTLSRANGENOISE_HPP_

namespace romea
{

class RTLSRangeNoise
{
public :

  RTLSRangeNoise();

  RTLSRangeNoise(const double & noiseA,
                 const double & noiseB);

  double computeStd(const double & range);

private:
  double a_;
  double b_;
};

}  // namespace romea

#endif  // ROMEA_CORE_RTLS_RTLSRANGENOISE_HPP_
