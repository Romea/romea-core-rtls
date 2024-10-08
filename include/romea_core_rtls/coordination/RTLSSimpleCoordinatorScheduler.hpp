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


#ifndef ROMEA_CORE_RTLS__COORDINATION__RTLSSIMPLECOORDINATORSCHEDULER_HPP_
#define ROMEA_CORE_RTLS__COORDINATION__RTLSSIMPLECOORDINATORSCHEDULER_HPP_

// std
#include <string>
#include <vector>
#include <functional>

// romea
#include "romea_core_common/time/Timer.hpp"
#include "romea_core_rtls_transceiver/RTLSTransceiverRangingResult.hpp"
#include "romea_core_rtls/coordination/RTLSTransceiversDiagnostics.hpp"


namespace romea
{
namespace core
{

class RTLSSimpleCoordinatorScheduler
{
public:
  using RangingResult = RTLSTransceiverRangingResult;
  using RangingRequestCallback = std::function<void (
        const size_t & /*initiarIndex*/,
        const size_t & /*responderIndex*/,
        const Duration & /*timeout*/)>;

public:
  RTLSSimpleCoordinatorScheduler(
    const double & poll_rate,
    const std::vector<std::string> & initiatorsNames,
    const std::vector<std::string> & respondersNames,
    RangingRequestCallback rangingCallback);

  virtual ~RTLSSimpleCoordinatorScheduler() = default;

  void start();

  void stop();

  void feedback(
    const size_t & initiatorIndex,
    const size_t & responderIndex,
    const RangingResult & result);

  virtual DiagnosticReport getReport();

protected:
  virtual void timerCallback_();

  virtual void incrementPollIndexes_();

protected:
  size_t numberOfInitiators_;
  size_t initiatorsPollIndex_;

  size_t numberOfResponders_;
  size_t respondersPollIndex_;

  Timer timer_;
  Duration timeout_;
  RangingRequestCallback rangingRequestCallback_;

  RTLSTransceiversDiagnostics diagnostics_;
};

}  // namespace core
}  // namespace romea

#endif   // ROMEA_CORE_RTLS__COORDINATION__RTLSSIMPLECOORDINATORSCHEDULER_HPP_
