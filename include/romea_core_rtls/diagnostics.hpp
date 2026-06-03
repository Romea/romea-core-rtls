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

#ifndef ROMEA_CORE_RTLS__DIAGNOSTICS_HPP_
#define ROMEA_CORE_RTLS__DIAGNOSTICS_HPP_

// std
#include <atomic>
#include <memory>
#include <string>
#include <vector>

// romea
#include "romea_core_common/diagnostic/CheckupReliability.hpp"
#include "romea_core_common/monitoring/OnlineAverage.hpp"
#include "romea_core_common/monitoring/RateMonitoring.hpp"
#include "romea_core_rtls/ranging/result.hpp"

namespace romea {
namespace core {

class RTLSTransceiversDiagnostics {
 public:
  RTLSTransceiversDiagnostics(const double& poll_rate,
                              const std::vector<std::string>& initiators_names,
                              const std::vector<std::string>& responders_names);

  void update(const size_t& initiators_poll_index,
              const size_t& responders_poll_index,
              const RTLSRangingResult& rangingResult);

  DiagnosticReport get_initiator_report(const size_t& initiator_index) const;
  DiagnosticReport get_responder_report(const size_t& responder_index) const;

 private:
  void init_initiators_diagnostics_(
      const double& poll_rate,
      const std::vector<std::string>& initiators_names);

  void init_responders_diagnostics_(
      const double& poll_rate,
      const std::vector<std::string>& responders_names);

  void update_initiator_reliability_(const double& reliability,
                                     const size_t& initiator_index);

  void update_responder_reliability_(const double& reliability,
                                     const size_t& responder_index);

 private:
  std::vector<std::unique_ptr<OnlineAverage>>
      responder_reliability_monitorings_;
  std::vector<std::unique_ptr<CheckupReliability>>
      responder_reliability_diagnostics_;
  std::vector<std::unique_ptr<OnlineAverage>>
      initiator_reliability_monitorings_;
  std::vector<std::unique_ptr<CheckupReliability>>
      initiator_reliability_diagnostics_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_RTLS__DIAGNOSTICS_HPP_
