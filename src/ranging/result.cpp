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
#include "romea_core_rtls/ranging/result.hpp"

namespace romea {
namespace core {

bool isEmpty(const RTLSRangingResult& result) {
  return result.range < std::numeric_limits<double>::epsilon() &&
         result.first_path_rx_power_level == 0 &&
         result.total_rx_power_level == 0;
}

}  // namespace core
}  // namespace romea
