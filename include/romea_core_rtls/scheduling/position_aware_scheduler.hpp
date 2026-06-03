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

#ifndef ROMEA_CORE_RTLS__SCHEDULING__POSITION_AWARE_SCHEDULER_HPP_
#define ROMEA_CORE_RTLS__SCHEDULING__POSITION_AWARE_SCHEDULER_HPP_

// Eigen
#include <Eigen/Core>

// std
#include <map>
#include <memory>
#include <string>
#include <vector>

// romea
#include "romea_core_common/containers/Eigen/VectorOfEigenVector.hpp"
#include "romea_core_common/time/Time.hpp"
#include "romea_core_rtls/scheduling/nearby_transceiver_finder.hpp"
#include "romea_core_rtls/scheduling/round_robin_scheduler.hpp"

namespace romea {
namespace core {

class RTLSPositionAwareScheduler : public RTLSRoundRobinScheduler {
 public:
  RTLSPositionAwareScheduler(const double& poll_rate,
                             const double& maximal_range,
                             const std::vector<std::string>& initiators_names,
                             const VectorOfEigenVector3d& initiators_positions,
                             const std::vector<std::string>& responders_names,
                             const VectorOfEigenVector3d& responders_positions,
                             RangingRequestCallback rangingRequestCallback);

  DiagnosticReport get_report() override;

  const std::vector<size_t> get_selected_responders_indexes();

  void update_robot_position(const Eigen::Vector3d& robotPosition);

 protected:
  void timer_callback_() override;

  void increment_poll_indexes_() override;

  void select_responders_();

 private:
  std::mutex mutex_;
  TimePoint last_robot_position_stamp_;
  Eigen::Vector3d last_robot_position_;
  RTLSNearbyTransceiverFinder reachable_responders_;
  std::vector<size_t> selected_responders_indexes_;
  size_t selected_responders_poll_index_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_RTLS__SCHEDULING__POSITION_AWARE_SCHEDULER_HPP_
