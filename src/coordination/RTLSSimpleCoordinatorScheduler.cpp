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


// std
#include <string>
#include <vector>
#include <iostream>

// romea
#include "romea_core_rtls/coordination/RTLSSimpleCoordinatorScheduler.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
RTLSSimpleCoordinatorScheduler::RTLSSimpleCoordinatorScheduler(
  const double & pollRate,
  const std::vector<std::string> & initiatorsNames,
  const std::vector<std::string> & respondersNames,
  RangingRequestCallback rangingRequestCallback)
: numberOfInitiators_(initiatorsNames.size()),
  initiatorsPollIndex_(initiatorsNames.size() - 1),
  numberOfResponders_(respondersNames.size()),
  respondersPollIndex_(respondersNames.size() - 1),
  timer_(std::bind(&RTLSSimpleCoordinatorScheduler::timerCallback_, this),
    durationFromSecond(1 / pollRate)),
  timeout_(durationFromSecond(pollRate) - durationFromMilliSecond(1)),
  rangingRequestCallback_(rangingRequestCallback),
  diagnostics_(pollRate, initiatorsNames, respondersNames)
{
}

//-----------------------------------------------------------------------------
void RTLSSimpleCoordinatorScheduler::start()
{
  timer_.start();
}

//-----------------------------------------------------------------------------
void RTLSSimpleCoordinatorScheduler::stop()
{
  timer_.stop();
}

//-----------------------------------------------------------------------------
void RTLSSimpleCoordinatorScheduler::timerCallback_()
{
  incrementPollIndexes_();
  rangingRequestCallback_(initiatorsPollIndex_, respondersPollIndex_, timeout_);
}

//-----------------------------------------------------------------------------
void RTLSSimpleCoordinatorScheduler::feedback(
  const size_t & initiatorIndex,
  const size_t & responderIndex,
  const RangingResult & result)
{
  diagnostics_.update(initiatorIndex, responderIndex, result);
}


//-----------------------------------------------------------------------------
void RTLSSimpleCoordinatorScheduler::incrementPollIndexes_()
{
  ++respondersPollIndex_;
  if (respondersPollIndex_ == numberOfResponders_) {
    respondersPollIndex_ = 0;
    ++initiatorsPollIndex_;
    if (initiatorsPollIndex_ == numberOfInitiators_) {
      initiatorsPollIndex_ = 0;
    }
  }
}

//-----------------------------------------------------------------------------
DiagnosticReport RTLSSimpleCoordinatorScheduler::getReport()
{
  DiagnosticReport report;
  for (size_t i = 0; i < numberOfInitiators_; ++i) {
    report += diagnostics_.getInitiatorReport(i);
  }

  for (size_t i = 0; i < numberOfResponders_; ++i) {
    report += diagnostics_.getResponderReport(i);
  }

  return report;
}

}  // namespace core
}  // namespace romea
