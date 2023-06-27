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
#include <string>
#include <utility>
#include <vector>
#include <limits>

// local
#include "romea_core_common/pointset/KdTree.hpp"
#include "romea_core_rtls/RTLSGeoreferencedCoordinator.hpp"


namespace romea
{

//-----------------------------------------------------------------------------
RTLSGeoreferencedCoordinator::RTLSGeoreferencedCoordinator(
  const double & pollRate,
  const double & minimalRange,
  const double & maximalRange,
  const uint8_t & rxPowerRejectionThreshold)
: poller_(maximalRange),
  rangingStatus_(minimalRange, maximalRange, rxPowerRejectionThreshold),
  transceiversDiagnostics_(pollRate)
{
}

//-----------------------------------------------------------------------------
void RTLSGeoreferencedCoordinator::setInitiatorsNames(
  const std::vector<std::string> initiatorsNames)
{
  poller_.setNumberOfInitiators(initiatorsNames.size());
  transceiversDiagnostics_.setInitiatorsNames(initiatorsNames);
}

//-----------------------------------------------------------------------------
void RTLSGeoreferencedCoordinator::setRespondersInfo(
  const std::vector<std::string> & respondersNames,
  const VectorOfEigenVector<Eigen::Vector3d> & responderPositions)
{
  poller_.setRespondersPositions(responderPositions);
  transceiversDiagnostics_.setRespondersNames(respondersNames);
}

//-----------------------------------------------------------------------------
std::optional<RTLSTransceiversPairIDs> RTLSGeoreferencedCoordinator::poll(const Duration & stamp)
{
  return poller_.poll(stamp);
}

//-----------------------------------------------------------------------------
void RTLSGeoreferencedCoordinator::updateRobotPosition(
  const Duration & stamp,
  const Eigen::Vector3d robotPosition)
{
  poller_.updateRobotPosition(stamp, robotPosition);
}


}  // namespace romea
