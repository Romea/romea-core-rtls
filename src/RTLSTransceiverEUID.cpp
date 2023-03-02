// Copyright 2022 INRAE, French National Research Institute for Agriculture, Food and Environment
// Add license

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
