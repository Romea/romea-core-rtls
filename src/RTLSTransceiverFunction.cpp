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


#include "romea_core_rtls/RTLSTransceiverFunction.hpp"

namespace romea
{

//-----------------------------------------------------------------------------
RTLSTransceiverFunction stringToFunction(const std::string & function)
{
  if (function == "listener") {
    return RTLSTransceiverFunction::LISTENER;
  } else if (function == "initiator") {
    return RTLSTransceiverFunction::INITIATOR;
  } else if (function == "responder") {
    return RTLSTransceiverFunction::RESPONDER;
  } else {
    return RTLSTransceiverFunction::NONE;
  }
}

//-----------------------------------------------------------------------------
std::string functionToString(const RTLSTransceiverFunction & function)
{
  switch (function) {
    case RTLSTransceiverFunction::LISTENER:
      return "listener";
    case RTLSTransceiverFunction::INITIATOR:
      return "initiator";
    case RTLSTransceiverFunction::RESPONDER:
      return "responder";
    default:
      return "none";
      break;
  }
}

}  // namespace romea
