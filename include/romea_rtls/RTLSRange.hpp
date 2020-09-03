#ifndef _romea_RTLSRange_hpp_
#define _romea_RTLSRange_hpp_

//romea
#include <romea_common/time/Time.hpp>

namespace romea
{

struct RTLSRange
{
  RTLSRange(const Duration & duration,
            const double & range,
            const unsigned char & firstPathRxPowerLevel,
            const unsigned char & totalRxPowerLevel):
    duration(duration),
    range(range),
    firstPathRxPowerLevel(firstPathRxPowerLevel),
    totalRxPowerLevel(totalRxPowerLevel)
  {

  }


  Duration duration;
  double range;
  unsigned char firstPathRxPowerLevel;
  unsigned char totalRxPowerLevel;
};

}


#endif
