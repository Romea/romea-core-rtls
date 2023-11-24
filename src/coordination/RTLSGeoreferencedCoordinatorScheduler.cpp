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
#include <vector>
#include <string>
#include <numeric>
#include <iostream>

// local
#include "romea_core_rtls/coordination/RTLSGeoreferencedCoordinatorScheduler.hpp"

namespace
{

double researchRadius(
  const double & maximalResearchDistance,
  const romea::core::VectorOfEigenVector3d & initiatorsPositions)
{
  double radius = 0;
  for (const auto & initiatorPosition : initiatorsPositions) {
    radius = std::max(radius, initiatorPosition.norm());
  }

  return radius + maximalResearchDistance;
}

}  // namespace

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
RTLSGeoreferencedCoordinatorScheduler::RTLSGeoreferencedCoordinatorScheduler(
  const double & pollRate,
  const double & maximalResearchDistance,
  const std::vector<std::string> & initiatorsNames,
  const VectorOfEigenVector3d & initiatorsPositions,
  const std::vector<std::string> & respondersNames,
  const VectorOfEigenVector3d & respondersPositions,
  RangingRequestCallback rangingRequestCallback)
: RTLSSimpleCoordinatorScheduler(
    pollRate,
    initiatorsNames,
    respondersNames,
    rangingRequestCallback),
  reachableResponders_(
    respondersPositions,
    researchRadius(maximalResearchDistance, initiatorsPositions)),
  selectedRespondersIndexes_(
    respondersNames.size()),
  selectedRespondersPollIndex_(
    respondersNames.size() - 1)
{
}

//-----------------------------------------------------------------------------
void RTLSGeoreferencedCoordinatorScheduler::timerCallback_()
{
  if (selectedRespondersIndexes_.size() >= 2) {
    incrementPollIndexes_();
  }

  if (initiatorsPollIndex_ == 0 && selectedRespondersPollIndex_ == 0) {
    selectResponders_();
  }

  if (selectedRespondersIndexes_.size() >= 2) {
    respondersPollIndex_ = selectedRespondersIndexes_[selectedRespondersPollIndex_];
    rangingRequestCallback_(initiatorsPollIndex_, respondersPollIndex_, timeout_);
  }
}

//-----------------------------------------------------------------------------
void RTLSGeoreferencedCoordinatorScheduler::incrementPollIndexes_()
{
  ++selectedRespondersPollIndex_;
  if (selectedRespondersPollIndex_ == selectedRespondersIndexes_.size()) {
    selectedRespondersPollIndex_ = 0;


    ++initiatorsPollIndex_;
    if (initiatorsPollIndex_ == numberOfInitiators_) {
      initiatorsPollIndex_ = 0;
    }
  }
}


//-----------------------------------------------------------------------------
void RTLSGeoreferencedCoordinatorScheduler::selectResponders_()
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (duration(now(), lastRobotPositionStamp_) < durationFromSecond(1)) {
    selectedRespondersIndexes_ = reachableResponders_.find(lastRobotPosition_);
  } else {
    selectedRespondersIndexes_.resize(numberOfResponders_);
    std::iota(selectedRespondersIndexes_.begin(), selectedRespondersIndexes_.end(), 0);
  }
}

//-----------------------------------------------------------------------------
void RTLSGeoreferencedCoordinatorScheduler::updateRobotPosition(
  const Eigen::Vector3d & robotPosition)
{
  std::lock_guard<std::mutex> lock(mutex_);
  lastRobotPosition_ = robotPosition;
  lastRobotPositionStamp_ = now();
}

//-----------------------------------------------------------------------------
const std::vector<size_t> RTLSGeoreferencedCoordinatorScheduler::getSelectedRespondersIndexes()
{
  return selectedRespondersIndexes_;
}

//-----------------------------------------------------------------------------
DiagnosticReport RTLSGeoreferencedCoordinatorScheduler::getReport()
{
  DiagnosticReport report;
  for (size_t i = 0; i < numberOfInitiators_; ++i) {
    report += diagnostics_.getInitiatorReport(i);
  }

  for (size_t i = 0; i < selectedRespondersIndexes_.size(); ++i) {
    report += diagnostics_.getResponderReport(selectedRespondersIndexes_[i]);
  }

  return report;
}

}  // namespace core
}  // namespace romea
