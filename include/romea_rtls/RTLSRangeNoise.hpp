#ifndef _romea_RTLSRangeNoise_hpp_
#define _romea_RTLSRangeNoise_hpp_

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



}

#endif
