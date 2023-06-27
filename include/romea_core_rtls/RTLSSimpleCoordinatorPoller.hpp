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


#ifndef ROMEA_RTLS__COORDINATORS_RTLSSIMPLECOORDINATORPOLLER_HPP_
#define ROMEA_RTLS__COORDINATORS_RTLSSIMPLECOORDINATORPOLLER_HPP_


// std
#include <optional>

// romea
#include "romea_core_rtls/RTLSTransceiversPairIDs.hpp"

namespace romea
{

class RTLSSimpleCoordinatorPoller
{
public:
  RTLSSimpleCoordinatorPoller();

  void setNumberOfInitiators(const size_t & numberOfInitiators);

  void setNumberOfResponders(const size_t & numberOfInitiators);

  virtual RTLSTransceiversPairIDs poll();

  virtual void reset();

private:
  size_t initiatorPollIndex_;
  size_t numberOfInitators_;
  size_t responderPollIndex_;
  size_t numberOfResponders_;
};

}  // namespace romea

#endif   // ROMEA_RTLS__COORDINATORS_RTLSCOORDINATORPOLLER_HPP_
