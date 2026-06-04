// Copyright 2022 INRAE, French National Research Institute for Agriculture,
// Food and Environment
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ROMEA_CORE_RTLS__RANGING__STATUS_HPP_
#define ROMEA_CORE_RTLS__RANGING__STATUS_HPP_

// romea
#include "romea_core_rtls/ranging/result.hpp"

namespace romea
{
namespace core
{

enum class RTLSRangingStatus
{
  FAILED = 0,
  UNAVAILABLE = 1,
  AVAILABLE = 2,
};

class RTLSRangingStatusEvaluator
{
public:
  RTLSRangingStatusEvaluator(
    const double & minimal_range,
    const double & maximal_range,
    const uint8_t & rx_power_rejection_threshold = 255);

  RTLSRangingStatus evaluate(const RTLSRangingResult & result);

private:
  double minimal_range_;
  double maximal_range_;
  uint8_t rx_power_rejection_threshold_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_RTLS__RANGING__STATUS_HPP_
