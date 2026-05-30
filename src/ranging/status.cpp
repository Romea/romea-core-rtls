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

// std
#include <limits>
// romea
#include "romea_core_rtls/ranging/status.hpp"

namespace romea {
namespace core {

//-----------------------------------------------------------------------------
RTLSRangingStatusEvaluator::RTLSRangingStatusEvaluator(
    const double& minimal_range, const double& maximal_range,
    const uint8_t& rx_power_rejection_threshold)
    : minimal_range_(minimal_range),
      maximal_range_(maximal_range),
      rx_power_rejection_threshold_(rx_power_rejection_threshold) {}

//-----------------------------------------------------------------------------
RTLSRangingStatus RTLSRangingStatusEvaluator::evaluate(
    const RTLSRangingResult& result) {
  if (isEmpty(result)) {
    return RTLSRangingStatus::FAILED;
  }

  if (result.range >= minimal_range_ && result.range <= maximal_range_ &&
      result.total_rx_power_level - result.first_path_rx_power_level <
          rx_power_rejection_threshold_) {
    return RTLSRangingStatus::AVAILABLE;
  } else {
    return RTLSRangingStatus::UNAVAILABLE;
  }
}

}  // namespace core
}  // namespace romea
