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

#ifndef ROMEA_CORE_RTLS__SCHEDULING__ROUND_ROBIN_SCHEDULER_HPP_
#define ROMEA_CORE_RTLS__SCHEDULING__ROUND_ROBIN_SCHEDULER_HPP_

// std
#include <functional>
#include <string>
#include <vector>

// romea
#include "romea_core_common/time/Timer.hpp"
#include "romea_core_rtls/diagnostics.hpp"
#include "romea_core_rtls/ranging/result.hpp"

namespace romea
{
namespace core
{

class RTLSRoundRobinScheduler
{
public:
  using RangingResult = RTLSRangingResult;
  using RangingRequestCallback = std::function<void(
    const size_t & /*initiar_index*/,
    const size_t & /*responder_index*/,
    const Duration & /*timeout*/)>;

public:
  RTLSRoundRobinScheduler(
    const double & poll_rate,
    const std::vector<std::string> & initiators_names,
    const std::vector<std::string> & responders_names,
    RangingRequestCallback ranging_callback);

  virtual ~RTLSRoundRobinScheduler() = default;

  void start();

  void stop();

  void feedback(
    const size_t & initiator_index, const size_t & responder_index, const RangingResult & result);

  virtual DiagnosticReport get_report();

protected:
  virtual void timer_callback_();

  virtual void increment_poll_indexes_();

protected:
  size_t number_of_initiators_;
  size_t initiators_poll_index_;

  size_t number_of_responders_;
  size_t responders_poll_index_;

  Timer timer_;
  Duration timeout_;
  RangingRequestCallback ranging_request_callback_;

  RTLSTransceiversDiagnostics diagnostics_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_RTLS__SCHEDULING__ROUND_ROBIN_SCHEDULER_HPP_
