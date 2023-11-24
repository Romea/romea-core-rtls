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

// std
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <string>

// romea
#include "romea_core_rtls/coordination/RTLSTransceiversDiagnostics.hpp"

namespace
{
const double DEFAULT_LOW_RELIABILITY_THRESHOLD = 0.3;
const double DEFAULT_HIGH_RELIABILITY_THRESHOLD = 0.8;
const double AVERAGE_MONITORING_PRECISION = 0.0001;
}

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
RTLSTransceiversDiagnostics::RTLSTransceiversDiagnostics(
  const double & pollRate,
  const std::vector<std::string> & initiatorsNames,
  const std::vector<std::string> & respondersNames)
: responderReliabilityMonitorings_(),
  responderReliabilityDiagnostics_(),
  initiatorReliabilityMonitorings_(),
  initiatorReliabilityDiagnostics_()
{
  initInitiatorsDiagnostics_(pollRate, initiatorsNames);
  initRespondersDiagnostics_(pollRate, respondersNames);
}

//-----------------------------------------------------------------------------
void RTLSTransceiversDiagnostics::initRespondersDiagnostics_(
  const double & pollRate,
  const std::vector<std::string> & respondersNames)
{
  responderReliabilityMonitorings_.clear();
  responderReliabilityDiagnostics_.clear();

  size_t responder_monitorings_window_size = 2 * pollRate / respondersNames.size();

  for (const std::string & responderName : respondersNames) {
    auto monitoring = std::make_unique<OnlineAverage>(
      AVERAGE_MONITORING_PRECISION,
      responder_monitorings_window_size);

    auto diagnostic = std::make_unique<CheckupReliability>(
      responderName,
      DEFAULT_LOW_RELIABILITY_THRESHOLD,
      DEFAULT_HIGH_RELIABILITY_THRESHOLD);

    responderReliabilityMonitorings_.push_back(std::move(monitoring));
    responderReliabilityDiagnostics_.push_back(std::move(diagnostic));
  }
}

//-----------------------------------------------------------------------------
void RTLSTransceiversDiagnostics::initInitiatorsDiagnostics_(
  const double & pollRate,
  const std::vector<std::string> & initiatorsNames)
{
  initiatorReliabilityMonitorings_.clear();
  initiatorReliabilityDiagnostics_.clear();

  size_t initiator_monitorings_window_size = 2 * pollRate / initiatorsNames.size();

  for (const std::string & initiator_name : initiatorsNames) {
    auto monitoring = std::make_unique<OnlineAverage>(
      AVERAGE_MONITORING_PRECISION,
      initiator_monitorings_window_size);

    auto diagnostic = std::make_unique<CheckupReliability>(
      initiator_name,
      DEFAULT_LOW_RELIABILITY_THRESHOLD,
      DEFAULT_HIGH_RELIABILITY_THRESHOLD);

    initiatorReliabilityMonitorings_.push_back(std::move(monitoring));
    initiatorReliabilityDiagnostics_.push_back(std::move(diagnostic));
  }
}

//-----------------------------------------------------------------------------
void RTLSTransceiversDiagnostics::update(
  const size_t & initiatorsPollIndex,
  const size_t & respondersPollIndex,
  const RTLSTransceiverRangingResult & rangingResult)
{
  assert(!initiatorReliabilityMonitorings_.empty());
  assert(!responderReliabilityMonitorings_.empty());

  if (!isEmpty(rangingResult)) {
    updateInitiatorReliability_(1, initiatorsPollIndex);
    updateResponderReliability_(1, respondersPollIndex);
  } else {
    updateInitiatorReliability_(1 / 3., initiatorsPollIndex);
    updateResponderReliability_(1 / 3., respondersPollIndex);
  }
}


//-----------------------------------------------------------------------------
void RTLSTransceiversDiagnostics::updateInitiatorReliability_(
  const double & reliability,
  const size_t & initiator_index)
{
  initiatorReliabilityMonitorings_[initiator_index]->update(reliability);
  initiatorReliabilityDiagnostics_[initiator_index]->evaluate(
    initiatorReliabilityMonitorings_[initiator_index]->getAverage());
}

//-----------------------------------------------------------------------------
void RTLSTransceiversDiagnostics::updateResponderReliability_(
  const double & reliability,
  const size_t & responder_index)
{
  responderReliabilityMonitorings_[responder_index]->update(reliability);
  responderReliabilityDiagnostics_[responder_index]->evaluate(
    responderReliabilityMonitorings_[responder_index]->getAverage());
}

//-----------------------------------------------------------------------------
DiagnosticReport RTLSTransceiversDiagnostics::getInitiatorReport(const size_t & initiatorIndex)
const
{
  return initiatorReliabilityDiagnostics_[initiatorIndex]->getReport();
}

//-----------------------------------------------------------------------------
DiagnosticReport RTLSTransceiversDiagnostics::getResponderReport(const size_t & responderIndex)
const
{
  return responderReliabilityDiagnostics_[responderIndex]->getReport();
}

}  // namespace core
}  // namespace romea
