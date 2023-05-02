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


#include "romea_core_rtls/RTLSTransceiverEUID.hpp"

namespace romea
{

//-----------------------------------------------------------------------------
bool operator==(
  const RTLSTransceiverEUID & euid1,
  const RTLSTransceiverEUID & euid2)
{
  return euid1.id == euid2.id && euid1.pan_id == euid2.pan_id;
}

//-----------------------------------------------------------------------------
bool operator<(
  const RTLSTransceiverEUID & euid1,
  const RTLSTransceiverEUID & euid2)
{
  return euid1.pan_id * 65535 + euid1.id < euid2.pan_id * 65535 + euid2.id;
}

}  // namespace romea
