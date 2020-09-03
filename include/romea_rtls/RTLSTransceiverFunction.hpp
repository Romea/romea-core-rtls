#ifndef _romea_RTLSTransceiverFunction_hpp_
#define _romea_RTLSTransceiverFunction_hpp_

#include <string>

namespace romea {

enum class RTLSTransceiverFunction
{
  INITIATOR,
  RESPONDER,
  LISTENER,
  NONE

};

RTLSTransceiverFunction stringToFunction(const std::string & function);

std::string functionToString(const RTLSTransceiverFunction & function);

}
#endif

