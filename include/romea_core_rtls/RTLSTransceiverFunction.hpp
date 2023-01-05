// Copyright 2022 INRAE, French National Research Institute for Agriculture, Food and Environment
// Add license

#ifndef ROMEA_CORE_RTLS__RTLSTRANSCEIVERFUNCTION_HPP_
#define ROMEA_CORE_RTLS__RTLSTRANSCEIVERFUNCTION_HPP_

#include <string>

namespace romea
{

enum class RTLSTransceiverFunction
{
  INITIATOR,
  RESPONDER,
  LISTENER,
  NONE
};

RTLSTransceiverFunction stringToFunction(const std::string & function);

std::string functionToString(const RTLSTransceiverFunction & function);

}  // namespace romea

#endif  // ROMEA_CORE_RTLS__RTLSTRANSCEIVERFUNCTION_HPP_
