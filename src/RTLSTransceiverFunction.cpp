#include "romea_rtls/RTLSTransceiverFunction.hpp"

namespace romea {

//-----------------------------------------------------------------------------
RTLSTransceiverFunction stringToFunction(const std::string & function)
{
  if(function=="listener")
  {
    return RTLSTransceiverFunction::LISTENER;
  }
  else if(function=="initiator")
  {
    return RTLSTransceiverFunction::INITIATOR;
  }
  else if(function=="responder")
  {
    return RTLSTransceiverFunction::RESPONDER;
  }
  else
  {
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

}

