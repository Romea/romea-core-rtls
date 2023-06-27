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

// local
#include "romea_core_rtls/RTLSSimpleCoordinator.hpp"

namespace romea
{

//-----------------------------------------------------------------------------
RTLSSimpleCoordinator::RTLSSimpleCoordinator(
  const double & pollRate,
  const double & minimalRange,
  const double & maximalRange,
  const uint8_t & rxPowerRejectionThreshold)
: rangingStatus_(minimalRange, maximalRange, rxPowerRejectionThreshold),
  transceiversDiagnostics_(pollRate)
{
}

//-----------------------------------------------------------------------------
void RTLSSimpleCoordinator::setInitiatorsNames(
  const std::vector<std::string> & initiatorsNames)
{
  poller_.setNumberOfInitiators(initiatorsNames.size());
  transceiversDiagnostics_.setInitiatorsNames(initiatorsNames);
}

//-----------------------------------------------------------------------------
void RTLSSimpleCoordinator::setRespondersNames(
  const std::vector<std::string> & respondersNames)
{
  poller_.setNumberOfResponders(respondersNames.size());
  transceiversDiagnostics_.setRespondersNames(respondersNames);
}

//-----------------------------------------------------------------------------
RTLSTransceiversPairIDs RTLSSimpleCoordinator::poll()
{
  return poller_.poll();
}

//-----------------------------------------------------------------------------
RTLSTransceiverRangingStatus RTLSSimpleCoordinator::feedback(
  const TransceiversIDs & transceiversIDs,
  const RangingResult & rangingResult)
{
  auto status = rangingStatus_.evaluate(rangingResult);
  transceiversDiagnostics_.update(transceiversIDs, status);
  return status;
}

//-----------------------------------------------------------------------------
DiagnosticReport RTLSSimpleCoordinator::getReport()
{
  return transceiversDiagnostics_.makeReport();
}


}  // namespace romea
