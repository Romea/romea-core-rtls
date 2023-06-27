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
#include <algorithm>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <cassert>

// local
#include "romea_core_rtls/RTLSTransceiversPairIDsMapping.hpp"

namespace romea
{


//-----------------------------------------------------------------------------
RTLSTransceiversPairIDsMapping::RTLSTransceiversPairIDsMapping()
: initiatorsNames_(),
  respondersNames_()
{
}

//-----------------------------------------------------------------------------
void RTLSTransceiversPairIDsMapping::setInitiatorsNames(
  const std::vector<std::string> & initiatorsNames)
{
  initiatorsNames_ = initiatorsNames;
}

//-----------------------------------------------------------------------------
void RTLSTransceiversPairIDsMapping::setRespondersNames(
  const std::vector<std::string> & respondersNames)
{
  respondersNames_ = respondersNames;
}

//-----------------------------------------------------------------------------
RTLSTransceiversPairIDs RTLSTransceiversPairIDsMapping::getTransceiversIDs(
  const RTLSTransceiversPairNames & transceiversNames)
{
  assert(!initiatorsNames_.empty());
  assert(!respondersNames_.empty());

  size_t initiatorIndex_ = findTransceiverIndex_(initiatorsNames_, transceiversNames.initiator);
  size_t responderIndex_ = findTransceiverIndex_(respondersNames_, transceiversNames.responder);
  return {initiatorIndex_, responderIndex_};
}

//-----------------------------------------------------------------------------
RTLSTransceiversPairNames RTLSTransceiversPairIDsMapping::getTransceiversNames(
  const RTLSTransceiversPairIDs & transceiverIDs)
{
  assert(!initiatorsNames_.empty());
  assert(!respondersNames_.empty());

  const std::string & initiatorName = initiatorsNames_[transceiverIDs.initiator];
  const std::string & responderName = respondersNames_[transceiverIDs.responder];
  return {initiatorName, responderName};
}

//-----------------------------------------------------------------------------
size_t RTLSTransceiversPairIDsMapping::findTransceiverIndex_(
  const std::vector<std::string> & transceiversNames,
  const std::string & transceiverName)
{
  auto iterator = std::find(
    std::cbegin(transceiversNames),
    std::cend(transceiversNames),
    transceiverName);

  assert(iterator != transceiversNames.end());

  return std::distance(std::cbegin(transceiversNames), iterator);
}

}  // namespace romea
