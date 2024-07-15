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


#ifndef ROMEA_CORE_RTLS__COORDINATION__RTLSGEOREFERENCEDCOORDINATORSCHEDULER_HPP_
#define ROMEA_CORE_RTLS__COORDINATION__RTLSGEOREFERENCEDCOORDINATORSCHEDULER_HPP_

// Eigen
#include <Eigen/Core>

// std
#include <map>
#include <memory>
#include <string>
#include <vector>

// romea
#include "romea_core_common/time/Time.hpp"
#include "romea_core_common/containers/Eigen/VectorOfEigenVector.hpp"
#include "romea_core_rtls/coordination/RTLSReachableTransceivers.hpp"
#include "romea_core_rtls/coordination/RTLSSimpleCoordinatorScheduler.hpp"


namespace romea
{
namespace core
{

class RTLSGeoreferencedCoordinatorScheduler : public RTLSSimpleCoordinatorScheduler
{
public:
  RTLSGeoreferencedCoordinatorScheduler(
    const double & pollRate,
    const double & maximalRange,
    const std::vector<std::string> & initiatorsNames,
    const VectorOfEigenVector3d & initiatorsPositions,
    const std::vector<std::string> & respondersNames,
    const VectorOfEigenVector3d & respondersPositions,
    RangingRequestCallback rangingRequestCallback);

  DiagnosticReport getReport() override;

  const std::vector<size_t> getSelectedRespondersIndexes();

  void updateRobotPosition(const Eigen::Vector3d & robotPosition);

protected:
  void timerCallback_() override;

  void incrementPollIndexes_() override;

  void selectResponders_();

private:
  std::mutex mutex_;
  TimePoint lastRobotPositionStamp_;
  Eigen::Vector3d lastRobotPosition_;
  RTLSReachableTransceivers reachableResponders_;
  std::vector<size_t> selectedRespondersIndexes_;
  size_t selectedRespondersPollIndex_;
};

}  // namespace core
}  // namespace romea

#endif   // ROMEA_CORE_RTLS__COORDINATION__RTLSGEOREFERENCEDCOORDINATORSCHEDULER_HPP_
