// Copyright 2022 INRAE, French National Research Institute for Agriculture,
// Food and Environment
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

// romea core
#include "romea_core_rtls/transceiver.hpp"

namespace romea
{
namespace core
{

//---------------------------------------------------------------------------
RTLSTransceiver::RTLSTransceiver(const EUID & euid, const Function & function)
: euid_(euid), function_(function)
{
}

//---------------------------------------------------------------------------
const RTLSTransceiver::EUID & RTLSTransceiver::get_euid()
{
  return euid_;
}

//---------------------------------------------------------------------------
const RTLSTransceiver::Function & RTLSTransceiver::get_function()
{
  return function_;
}

//-----------------------------------------------------------------------------
RTLSTransceiver::Function string_to_function(const std::string & function)
{
  if (function == "listener") {
    return RTLSTransceiver::Function::LISTENER;
  } else if (function == "initiator") {
    return RTLSTransceiver::Function::INITIATOR;
  } else if (function == "responder") {
    return RTLSTransceiver::Function::RESPONDER;
  } else {
    return RTLSTransceiver::Function::NONE;
  }
}

//-----------------------------------------------------------------------------
std::string function_to_string(const RTLSTransceiver::Function & function)
{
  switch (function) {
    case RTLSTransceiver::Function::LISTENER:
      return "listener";
    case RTLSTransceiver::Function::INITIATOR:
      return "initiator";
    case RTLSTransceiver::Function::RESPONDER:
      return "responder";
    default:
      return "none";
      break;
  }
}

//-----------------------------------------------------------------------------
bool operator==(const RTLSTransceiver::EUID & euid1, const RTLSTransceiver::EUID & euid2)
{
  return euid1.id == euid2.id && euid1.pan_id == euid2.pan_id;
}

//-----------------------------------------------------------------------------
bool operator<(const RTLSTransceiver::EUID & euid1, const RTLSTransceiver::EUID & euid2)
{
  if (euid1.pan_id != euid2.pan_id) {
    return euid1.pan_id < euid2.pan_id;
  }

  return euid1.id < euid2.id;
}

}  // namespace core
}  // namespace romea
