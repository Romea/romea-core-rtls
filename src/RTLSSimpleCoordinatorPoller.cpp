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
#include <cassert>

// local
#include "romea_core_rtls/RTLSSimpleCoordinatorPoller.hpp"


namespace romea
{

//-----------------------------------------------------------------------------
RTLSSimpleCoordinatorPoller::RTLSSimpleCoordinatorPoller()
: initiatorPollIndex_(0),
  numberOfInitators_(0),
  responderPollIndex_(0),
  numberOfResponders_(0)
{
}

//-----------------------------------------------------------------------------
void RTLSSimpleCoordinatorPoller::setNumberOfInitiators(const size_t & numberOfInitiators)
{
  reset();
  numberOfInitators_ = numberOfInitiators;
}

//-----------------------------------------------------------------------------
void RTLSSimpleCoordinatorPoller::setNumberOfResponders(const size_t & numberOfResponders)
{
  reset();
  numberOfResponders_ = numberOfResponders;
}

//-----------------------------------------------------------------------------
RTLSTransceiversPairIDs RTLSSimpleCoordinatorPoller::poll()
{
  assert(numberOfInitators_ != 0);
  assert(numberOfResponders_ != 0);

  auto poll = RTLSTransceiversPairIDs{initiatorPollIndex_, responderPollIndex_};

  ++responderPollIndex_;
  if (responderPollIndex_ == numberOfResponders_) {
    responderPollIndex_ = 0;

    ++initiatorPollIndex_;
    if (initiatorPollIndex_ == numberOfInitators_) {
      initiatorPollIndex_ = 0;
    }
  }

  return poll;
}


//-----------------------------------------------------------------------------
void RTLSSimpleCoordinatorPoller::reset()
{
  initiatorPollIndex_ = 0;
  responderPollIndex_ = 0;
}

}  // namespace romea
