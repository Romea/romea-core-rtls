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


#ifndef ROMEA_RTLS__COORDINATORS_RTLSGEOREFERENCEDCOORDINATORPOLLER_HPP_
#define ROMEA_RTLS__COORDINATORS_RTLSGEOREFERENCEDCOORDINATORPOLLER_HPP_

// std
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <mutex>

// romea
#include "romea_core_common/time/Time.hpp"
#include "romea_core_common/pointset/KdTree.hpp"
#include "romea_core_rtls/RTLSTransceiversPairIDs.hpp"

namespace romea
{

class RTLSGeoreferencedCoordinatorPoller
{
public:
  explicit RTLSGeoreferencedCoordinatorPoller(const double & maximalResponderDistance);

  void setNumberOfInitiators(const size_t & numberOfInitiators);

  void setRespondersPositions(const VectorOfEigenVector<Eigen::Vector3d> & responderPositions);

  std::optional<RTLSTransceiversPairIDs> poll(const Duration & stamp);

  void updateRobotPosition(
    const Duration & stamp,
    const Eigen::Vector3d robotPosition);

  void reset();

protected:
  std::unique_ptr<romea::KdTree<Eigen::Vector3d>> responderPositionsKDTree_;

  size_t initiatorPollIndex_;
  size_t numberOfInitiators_;

  size_t numberOfResponders_;
  size_t nearestResponderPollIndex_;
  std::vector<size_t> nearestResponderIndexes_;
  std::vector<double> nearestResponderSquareDistances_;
  double maximalResponderDistance_;

  std::mutex mutex_;
  Eigen::Vector3d lastRobotPosition_;
  Duration lastRobotPositionStamp_;
};

}  // namespace romea

#endif   // ROMEA_RTLS__COORDINATORS_RTLSCOORDINATORPOLLER_HPP_
