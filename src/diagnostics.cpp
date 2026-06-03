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

// std
#include <memory>
#include <string>
#include <utility>
#include <vector>

// romea
#include "romea_core_rtls/diagnostics.hpp"

namespace {
const double DEFAULT_LOW_RELIABILITY_THRESHOLD = 0.3;
const double DEFAULT_HIGH_RELIABILITY_THRESHOLD = 0.8;
const double AVERAGE_MONITORING_PRECISION = 0.0001;
}  // namespace

namespace romea {
namespace core {

//-----------------------------------------------------------------------------
RTLSTransceiversDiagnostics::RTLSTransceiversDiagnostics(
    const double& poll_rate, const std::vector<std::string>& initiators_names,
    const std::vector<std::string>& responders_names)
    : responder_reliability_monitorings_(),
      responder_reliability_diagnostics_(),
      initiator_reliability_monitorings_(),
      initiator_reliability_diagnostics_() {
  init_initiators_diagnostics_(poll_rate, initiators_names);
  init_responders_diagnostics_(poll_rate, responders_names);
}

//-----------------------------------------------------------------------------
void RTLSTransceiversDiagnostics::init_responders_diagnostics_(
    const double& poll_rate, const std::vector<std::string>& responders_names) {
  responder_reliability_monitorings_.clear();
  responder_reliability_diagnostics_.clear();

  size_t responder_monitorings_window_size =
      2 * poll_rate / responders_names.size();

  for (const std::string& responderName : responders_names) {
    auto monitoring = std::make_unique<OnlineAverage>(
        AVERAGE_MONITORING_PRECISION, responder_monitorings_window_size);

    auto diagnostic = std::make_unique<CheckupReliability>(
        responderName, DEFAULT_LOW_RELIABILITY_THRESHOLD,
        DEFAULT_HIGH_RELIABILITY_THRESHOLD);

    responder_reliability_monitorings_.push_back(std::move(monitoring));
    responder_reliability_diagnostics_.push_back(std::move(diagnostic));
  }
}

//-----------------------------------------------------------------------------
void RTLSTransceiversDiagnostics::init_initiators_diagnostics_(
    const double& poll_rate, const std::vector<std::string>& initiators_names) {
  initiator_reliability_monitorings_.clear();
  initiator_reliability_diagnostics_.clear();

  size_t initiator_monitorings_window_size =
      2 * poll_rate / initiators_names.size();

  for (const std::string& initiator_name : initiators_names) {
    auto monitoring = std::make_unique<OnlineAverage>(
        AVERAGE_MONITORING_PRECISION, initiator_monitorings_window_size);

    auto diagnostic = std::make_unique<CheckupReliability>(
        initiator_name, DEFAULT_LOW_RELIABILITY_THRESHOLD,
        DEFAULT_HIGH_RELIABILITY_THRESHOLD);

    initiator_reliability_monitorings_.push_back(std::move(monitoring));
    initiator_reliability_diagnostics_.push_back(std::move(diagnostic));
  }
}

//-----------------------------------------------------------------------------
void RTLSTransceiversDiagnostics::update(
    const size_t& initiators_poll_index, const size_t& responders_poll_index,
    const RTLSRangingResult& rangingResult) {
  assert(!initiator_reliability_monitorings_.empty());
  assert(!responder_reliability_monitorings_.empty());

  if (!isEmpty(rangingResult)) {
    update_initiator_reliability_(1, initiators_poll_index);
    update_responder_reliability_(1, responders_poll_index);
  } else {
    update_initiator_reliability_(1 / 3., initiators_poll_index);
    update_responder_reliability_(1 / 3., responders_poll_index);
  }
}

//-----------------------------------------------------------------------------
void RTLSTransceiversDiagnostics::update_initiator_reliability_(
    const double& reliability, const size_t& initiator_index) {
  initiator_reliability_monitorings_[initiator_index]->update(reliability);
  initiator_reliability_diagnostics_[initiator_index]->evaluate(
      initiator_reliability_monitorings_[initiator_index]->getAverage());
}

//-----------------------------------------------------------------------------
void RTLSTransceiversDiagnostics::update_responder_reliability_(
    const double& reliability, const size_t& responder_index) {
  responder_reliability_monitorings_[responder_index]->update(reliability);
  responder_reliability_diagnostics_[responder_index]->evaluate(
      responder_reliability_monitorings_[responder_index]->getAverage());
}

//-----------------------------------------------------------------------------
DiagnosticReport RTLSTransceiversDiagnostics::get_initiator_report(
    const size_t& initiator_index) const {
  return initiator_reliability_diagnostics_[initiator_index]->getReport();
}

//-----------------------------------------------------------------------------
DiagnosticReport RTLSTransceiversDiagnostics::get_responder_report(
    const size_t& responder_index) const {
  return responder_reliability_diagnostics_[responder_index]->getReport();
}

}  // namespace core
}  // namespace romea
