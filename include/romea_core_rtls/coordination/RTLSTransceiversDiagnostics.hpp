// Copyright 2022 INRAE, French National Research Institute for Agriculture, Food and Environment
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

#ifndef ROMEA_RTLS__COORDINATORS__RTLSCOORDINATORDIAGNOSTICS_HPP_
#define ROMEA_RTLS__COORDINATORS__RTLSCOORDINATORDIAGNOSTICS_HPP_

// std
#include <atomic>
#include <memory>
#include <vector>
#include <string>

// romea
#include "romea_core_common/monitoring/OnlineAverage.hpp"
#include "romea_core_common/monitoring/RateMonitoring.hpp"
#include "romea_core_common/diagnostic/CheckupReliability.hpp"
#include "romea_core_rtls_transceiver/RTLSTransceiverRangingResult.hpp"

namespace romea
{

class RTLSTransceiversDiagnostics
{
public:
  RTLSTransceiversDiagnostics(
    const double & pollRate,
    const std::vector<std::string> & initiatorsNames,
    const std::vector<std::string> & respondersNames);

  void update(
    const size_t & initiatorsPollIndex,
    const size_t & respondersPollIndex,
    const RTLSTransceiverRangingResult & rangingResult);

  DiagnosticReport getInitiatorReport(const size_t & initiatorIndex) const;
  DiagnosticReport getResponderReport(const size_t & responderIndex) const;

private:
  void initInitiatorsDiagnostics_(
    const double & pollRate,
    const std::vector<std::string> & initiatorsNames);

  void initRespondersDiagnostics_(
    const double & pollRate,
    const std::vector<std::string> & respondersNames);

  void updateInitiatorReliability_(
    const double & reliability,
    const size_t & initiatorIndex);

  void updateResponderReliability_(
    const double & reliability,
    const size_t & responderIndex);

private:
  std::vector<std::unique_ptr<OnlineAverage>> responderReliabilityMonitorings_;
  std::vector<std::unique_ptr<CheckupReliability>> responderReliabilityDiagnostics_;
  std::vector<std::unique_ptr<OnlineAverage>> initiatorReliabilityMonitorings_;
  std::vector<std::unique_ptr<CheckupReliability>> initiatorReliabilityDiagnostics_;
};

}  // namespace romea

#endif  // ROMEA_RTLS__COORDINATORS__RTLSCOORDINATORDIAGNOSTICS_HPP_
