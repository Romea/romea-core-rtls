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


#ifndef ROMEA_RTLS__COORDINATORS_RTLSCOORDINATOR_HPP_
#define ROMEA_RTLS__COORDINATORS_RTLSCOORDINATOR_HPP_

// Eigen
#include <Eigen/Core>

// std
#include <map>
#include <memory>
#include <string>
#include <vector>

// romea
#include "romea_core_rtls/RTLSTransceiversDiagnostics.hpp"
#include "romea_core_rtls/RTLSSimpleCoordinatorPoller.hpp"
#include "romea_core_rtls_transceiver/RTLSTransceiverRangingStatus.hpp"

namespace romea
{

class RTLSSimpleCoordinator
{
private:
  using RangingStatus = RTLSTransceiverRangingStatus;
  using RangingResult = RTLSTransceiverRangingResult;
  using TransceiversIDs = RTLSTransceiversPairIDs;

public:
  RTLSSimpleCoordinator(
    const double & pollRate,
    const double & minimalRange,
    const double & maximalRange,
    const uint8_t & rxPowerRejectionThreshold);

  void setInitiatorsNames(const std::vector<std::string> & initiatorNames);

  void setRespondersNames(const std::vector<std::string> & responderNames);

  TransceiversIDs poll();

  RangingStatus feedback(
    const TransceiversIDs & transceiversIDs,
    const RangingResult & rangingResult);

  DiagnosticReport getReport();

protected:
  RTLSSimpleCoordinatorPoller poller_;
  RTLSTransceiverRangingStatusEvaluator rangingStatus_;
  RTLSTransceiversDiagnostics transceiversDiagnostics_;
};

}  // namespace romea

#endif   // ROMEA_RTLS__COORDINATORS_RTLSCOORDINATOR_HPP_
