// Copyright 2022 INRAE, French National Research Institute for Agriculture, Food and Environment
// Add license

#ifndef ROMEA_CORE_RTLS__RTLSRANGE_HPP_
#define ROMEA_CORE_RTLS__RTLSRANGE_HPP_

// romea
#include <romea_core_common/time/Time.hpp>

namespace romea
{

struct RTLSRange
{
  Duration duration;
  double range;
  unsigned char firstPathRxPowerLevel;
  unsigned char totalRxPowerLevel;
};

}  // namespace romea


#endif  // ROMEA_CORE_RTLS__RTLSRANGE_HPP_
