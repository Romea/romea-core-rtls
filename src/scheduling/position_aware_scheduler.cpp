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
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// local
#include "romea_core_rtls/scheduling/position_aware_scheduler.hpp"

namespace {

double researchRadius(
    const double& maximalResearchDistance,
    const romea::core::VectorOfEigenVector3d& initiators_positions) {
  double radius = 0;
  for (const auto& initiatorPosition : initiators_positions) {
    radius = std::max(radius, initiatorPosition.norm());
  }

  return radius + maximalResearchDistance;
}

}  // namespace

namespace romea {
namespace core {

//-----------------------------------------------------------------------------
RTLSPositionAwareScheduler::RTLSPositionAwareScheduler(
    const double& pollRate, const double& maximalResearchDistance,
    const std::vector<std::string>& initiators_names,
    const VectorOfEigenVector3d& initiators_positions,
    const std::vector<std::string>& responders_names,
    const VectorOfEigenVector3d& respondersPositions,
    RangingRequestCallback rangingRequestCallback)
    : RTLSRoundRobinScheduler(pollRate, initiators_names, responders_names,
                              rangingRequestCallback),
      reachable_responders_(
          respondersPositions,
          researchRadius(maximalResearchDistance, initiators_positions)),
      selected_responders_indexes_(responders_names.size()),
      selected_responders_poll_index_(responders_names.size() - 1) {}

//-----------------------------------------------------------------------------
void RTLSPositionAwareScheduler::timer_callback_() {
  if (selected_responders_indexes_.size() >= 2) {
    increment_poll_indexes_();
  }

  if (initiators_poll_index_ == 0 && selected_responders_poll_index_ == 0) {
    select_responders_();
  }

  if (selected_responders_indexes_.size() >= 2) {
    responders_poll_index_ =
        selected_responders_indexes_[selected_responders_poll_index_];
    ranging_request_callback_(initiators_poll_index_, responders_poll_index_,
                              timeout_);
  }
}

//-----------------------------------------------------------------------------
void RTLSPositionAwareScheduler::increment_poll_indexes_() {
  ++selected_responders_poll_index_;
  if (selected_responders_poll_index_ == selected_responders_indexes_.size()) {
    selected_responders_poll_index_ = 0;

    ++initiators_poll_index_;
    if (initiators_poll_index_ == number_of_initiators_) {
      initiators_poll_index_ = 0;
    }
  }
}

//-----------------------------------------------------------------------------
void RTLSPositionAwareScheduler::select_responders_() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (duration(now(), last_robot_position_stamp_) < durationFromSecond(1)) {
    selected_responders_indexes_ =
        reachable_responders_.find(last_robot_position_);
  } else {
    selected_responders_indexes_.resize(number_of_responders_);
    std::iota(selected_responders_indexes_.begin(),
              selected_responders_indexes_.end(), 0);
  }
}

//-----------------------------------------------------------------------------
void RTLSPositionAwareScheduler::update_robot_position(
    const Eigen::Vector3d& robotPosition) {
  std::lock_guard<std::mutex> lock(mutex_);
  last_robot_position_ = robotPosition;
  last_robot_position_stamp_ = now();
}

//-----------------------------------------------------------------------------
const std::vector<size_t>
RTLSPositionAwareScheduler::get_selected_responders_indexes() {
  return selected_responders_indexes_;
}

//-----------------------------------------------------------------------------
DiagnosticReport RTLSPositionAwareScheduler::get_report() {
  DiagnosticReport report;
  for (size_t i = 0; i < number_of_initiators_; ++i) {
    report += diagnostics_.get_initiator_report(i);
  }

  for (size_t i = 0; i < selected_responders_indexes_.size(); ++i) {
    report +=
        diagnostics_.get_responder_report(selected_responders_indexes_[i]);
  }

  return report;
}

}  // namespace core
}  // namespace romea
