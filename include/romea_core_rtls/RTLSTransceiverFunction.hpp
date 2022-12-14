#ifndef ROMEA_CORE_RTLS_RTLSTRANSCEIVERFUNCTION_HPP_
#define ROMEA_CORE_RTLS_RTLSTRANSCEIVERFUNCTION_HPP_

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

}  // namespace romea

#endif  // ROMEA_CORE_RTLS_RTLSTRANSCEIVERFUNCTION_HPP_

