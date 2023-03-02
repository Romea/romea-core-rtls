#ifndef ROMEA_CORE_RTLS__RTLSTRANSCEIVEREUID_HPP_
#define ROMEA_CORE_RTLS__RTLSTRANSCEIVEREUID_HPP_

#include <cstdint>

namespace romea
{
struct RTLSTransceiverEUID
{
  uint16_t pan_id;
  uint16_t id;
};

bool operator==(
  const RTLSTransceiverEUID & euid1,
  const RTLSTransceiverEUID & euid2);

bool operator<(
  const RTLSTransceiverEUID & euid1,
  const RTLSTransceiverEUID & euid2);

}  // namespace romea

#endif  // ROMEA_CORE_RTLS__RTLSTRANSCEIVEREUID_HPP_
