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
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// romea
#include "romea_core_rtls/scheduling/round_robin_scheduler.hpp"

namespace romea
{
namespace core
{

namespace
{

void validate_scheduler_inputs(
  const double & poll_rate,
  const std::vector<std::string> & initiators_names,
  const std::vector<std::string> & responders_names)
{
  if (poll_rate <= 0.0) {
    throw std::invalid_argument("RTLSRoundRobinScheduler poll_rate must be strictly positive.");
  }

  if (initiators_names.empty()) {
    throw std::invalid_argument("RTLSRoundRobinScheduler requires at least one initiator.");
  }

  if (responders_names.empty()) {
    throw std::invalid_argument("RTLSRoundRobinScheduler requires at least one responder.");
  }
}

}  // namespace

//-----------------------------------------------------------------------------
RTLSRoundRobinScheduler::RTLSRoundRobinScheduler(
  const double & poll_rate,
  const std::vector<std::string> & initiators_names,
  const std::vector<std::string> & responders_names,
  RangingRequestCallback rangingRequestCallback)
: number_of_initiators_(
    (validate_scheduler_inputs(poll_rate, initiators_names, responders_names),
     initiators_names.size())),
  initiators_poll_index_(initiators_names.size() - 1),
  number_of_responders_(responders_names.size()),
  responders_poll_index_(responders_names.size() - 1),
  timer_(
    std::bind(&RTLSRoundRobinScheduler::timer_callback_, this), durationFromSecond(1 / poll_rate)),
  timeout_(durationFromSecond(1 / poll_rate) - durationFromMilliSecond(1)),
  ranging_request_callback_(rangingRequestCallback),
  diagnostics_(poll_rate, initiators_names, responders_names)
{
}

//-----------------------------------------------------------------------------
void RTLSRoundRobinScheduler::start()
{
  timer_.start();
}

//-----------------------------------------------------------------------------
void RTLSRoundRobinScheduler::stop()
{
  timer_.stop();
}

//-----------------------------------------------------------------------------
void RTLSRoundRobinScheduler::timer_callback_()
{
  increment_poll_indexes_();
  ranging_request_callback_(initiators_poll_index_, responders_poll_index_, timeout_);
}

//-----------------------------------------------------------------------------
void RTLSRoundRobinScheduler::feedback(
  const size_t & initiator_index, const size_t & responder_index, const RangingResult & result)
{
  diagnostics_.update(initiator_index, responder_index, result);
}

//-----------------------------------------------------------------------------
void RTLSRoundRobinScheduler::increment_poll_indexes_()
{
  ++responders_poll_index_;
  if (responders_poll_index_ == number_of_responders_) {
    responders_poll_index_ = 0;
    ++initiators_poll_index_;
    if (initiators_poll_index_ == number_of_initiators_) {
      initiators_poll_index_ = 0;
    }
  }
}

//-----------------------------------------------------------------------------
DiagnosticReport RTLSRoundRobinScheduler::get_report()
{
  DiagnosticReport report;
  for (size_t i = 0; i < number_of_initiators_; ++i) {
    report += diagnostics_.get_initiator_report(i);
  }

  for (size_t i = 0; i < number_of_responders_; ++i) {
    report += diagnostics_.get_responder_report(i);
  }

  return report;
}

}  // namespace core
}  // namespace romea
