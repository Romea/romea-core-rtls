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
#include <memory>
#include <vector>
#include <numeric>

// local
#include "romea_core_rtls/RTLSGeoreferencedCoordinatorPoller.hpp"


namespace romea
{

//-----------------------------------------------------------------------------
RTLSGeoreferencedCoordinatorPoller::RTLSGeoreferencedCoordinatorPoller(
  const double & maximalResponderDistance)
: initiatorPollIndex_(0),
  numberOfInitiators_(0),
  numberOfResponders_(0),
  nearestResponderPollIndex_(0),
  nearestResponderIndexes_(),
  nearestResponderSquareDistances_(),
  maximalResponderDistance_(maximalResponderDistance),
  mutex_(),
  lastRobotPosition_(Eigen::Vector3d::Zero()),
  lastRobotPositionStamp_()
{
}

//-----------------------------------------------------------------------------
void RTLSGeoreferencedCoordinatorPoller::setNumberOfInitiators(const size_t & numberOfInitiators)
{
  reset();
  numberOfInitiators_ = numberOfInitiators;
}

//-----------------------------------------------------------------------------
void RTLSGeoreferencedCoordinatorPoller::setRespondersPositions(
  const VectorOfEigenVector<Eigen::Vector3d> & respondersPositions)
{
  reset();

  responderPositionsKDTree_ = std::make_unique<KdTree<Eigen::Vector3d>>(respondersPositions);
  nearestResponderSquareDistances_.reserve(respondersPositions.size());
  nearestResponderIndexes_.reserve(respondersPositions.size());
  numberOfResponders_ = respondersPositions.size();
}

//-----------------------------------------------------------------------------
std::optional<RTLSTransceiversPairIDs> RTLSGeoreferencedCoordinatorPoller::poll(
  const Duration & stamp)
{
  if (numberOfResponders_ != 0 && numberOfInitiators_ != 0) {
    if (nearestResponderPollIndex_ == 0 && initiatorPollIndex_ == 0) {
      std::lock_guard<std::mutex> lock(mutex_);
      if (durationToSecond(stamp - lastRobotPositionStamp_) > 1) {
        responderPositionsKDTree_->radiusResearch(
          lastRobotPosition_,
          maximalResponderDistance_ * maximalResponderDistance_,
          nearestResponderIndexes_,
          nearestResponderSquareDistances_);
      } else {
        nearestResponderIndexes_.resize(numberOfResponders_);
        std::iota(std::begin(nearestResponderIndexes_), std::end(nearestResponderIndexes_), 0);
      }
    }

    if (nearestResponderIndexes_.size() >= 2) {
      auto poll = RTLSTransceiversPairIDs{initiatorPollIndex_,
        nearestResponderIndexes_[nearestResponderPollIndex_]};

      ++initiatorPollIndex_;
      if (initiatorPollIndex_ == numberOfInitiators_) {
        initiatorPollIndex_ = 0;

        ++nearestResponderPollIndex_;
        if (nearestResponderPollIndex_ == nearestResponderIndexes_.size()) {
          nearestResponderPollIndex_ = 0;
        }
      }

      return poll;
    }
  }

  return std::nullopt;
}

//-----------------------------------------------------------------------------
void RTLSGeoreferencedCoordinatorPoller::updateRobotPosition(
  const Duration & stamp,
  const Eigen::Vector3d robotPosition)
{
  std::lock_guard<std::mutex> lock(mutex_);
  lastRobotPosition_ = robotPosition;
  lastRobotPositionStamp_ = stamp;
}

//-----------------------------------------------------------------------------
void RTLSGeoreferencedCoordinatorPoller::reset()
{
  initiatorPollIndex_ = 0;
  nearestResponderPollIndex_ = 0;
}


}  // namespace romea
