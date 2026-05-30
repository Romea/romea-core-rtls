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
#include <map>
#include <memory>
#include <string>
#include <vector>

// gtest
#include "gtest/gtest.h"

// romea
#include "romea_core_rtls/scheduling/position_aware_scheduler.hpp"

class TestPositionAwareScheduler : public ::testing::Test {
 protected:
  TestPositionAwareScheduler()
      : scheduler_(nullptr), initiators_indexes_(), responders_indexes_() {}

  void init(const double& pollRate, const double& maximalResearchDistance) {
    std::vector<std::string> initiators_names{"initiator0", "initiator1"};

    romea::core::VectorOfEigenVector3d initiatorsPositions = {
        Eigen::Vector3d(1.0, 0.5, 2.0), Eigen::Vector3d(-1.0, -0.5, 2.0)};

    std::vector<std::string> responders_names(
        {"responder0", "responder1", "responder2"});

    romea::core::VectorOfEigenVector3d respondersPositions = {
        Eigen::Vector3d(-10.0, 0.0, 2.0), Eigen::Vector3d(0.0, 0.0, 2.0),
        Eigen::Vector3d(10.0, 0.0, 2.0)};

    auto callback = [this](const size_t& initiator_index,
                           const size_t& responder_index,
                           const romea::core::Duration& /*timeout*/) {
      initiators_indexes_.push_back(initiator_index);
      responders_indexes_.push_back(responder_index);
      return romea::core::RTLSRangingResult();
    };

    scheduler_ = std::make_unique<romea::core::RTLSPositionAwareScheduler>(
        pollRate, maximalResearchDistance, initiators_names,
        initiatorsPositions, responders_names, respondersPositions, callback);
  }

  std::unique_ptr<romea::core::RTLSPositionAwareScheduler> scheduler_;
  std::vector<size_t> initiators_indexes_;
  std::vector<size_t> responders_indexes_;
};

TEST_F(TestPositionAwareScheduler, checkPollWhenNoPositionIsGiven) {
  init(30, 20);

  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromSecond(2));
  scheduler_->stop();

  EXPECT_EQ(initiators_indexes_[6], 0);
  EXPECT_EQ(responders_indexes_[6], 0);
  EXPECT_EQ(initiators_indexes_[7], 0);
  EXPECT_EQ(responders_indexes_[7], 1);
  EXPECT_EQ(initiators_indexes_[8], 0);
  EXPECT_EQ(responders_indexes_[8], 2);
  EXPECT_EQ(initiators_indexes_[9], 1);
  EXPECT_EQ(responders_indexes_[9], 0);
  EXPECT_EQ(initiators_indexes_[10], 1);
  EXPECT_EQ(responders_indexes_[10], 1);
  EXPECT_EQ(initiators_indexes_[11], 1);
  EXPECT_EQ(responders_indexes_[11], 2);
  EXPECT_EQ(initiators_indexes_[12], 0);
  EXPECT_EQ(responders_indexes_[12], 0);
  EXPECT_EQ(initiators_indexes_.size(), 60);
  EXPECT_EQ(responders_indexes_.size(), 60);
}

TEST_F(TestPositionAwareScheduler, checkPollWhenAllRespondersAreVisible) {
  init(30, 20);
  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(100));
  scheduler_->update_robot_position(Eigen::Vector3d::Zero());
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(1900));
  scheduler_->stop();

  EXPECT_EQ(initiators_indexes_[6], 0);
  EXPECT_EQ(responders_indexes_[6], 0);
  EXPECT_EQ(initiators_indexes_[7], 0);
  EXPECT_EQ(responders_indexes_[7], 1);
  EXPECT_EQ(initiators_indexes_[8], 0);
  EXPECT_EQ(responders_indexes_[8], 2);
  EXPECT_EQ(initiators_indexes_[9], 1);
  EXPECT_EQ(responders_indexes_[9], 0);
  EXPECT_EQ(initiators_indexes_[10], 1);
  EXPECT_EQ(responders_indexes_[10], 1);
  EXPECT_EQ(initiators_indexes_[11], 1);
  EXPECT_EQ(responders_indexes_[11], 2);
  EXPECT_EQ(initiators_indexes_[12], 0);
  EXPECT_EQ(responders_indexes_[12], 0);
  EXPECT_EQ(initiators_indexes_.size(), 60);
  EXPECT_EQ(responders_indexes_.size(), 60);
}

TEST_F(TestPositionAwareScheduler, checkPollWhenTwoRespondersAreVisible) {
  init(30, 20);
  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(100));
  scheduler_->update_robot_position(Eigen::Vector3d(13, 0, 0));
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(1900));
  scheduler_->stop();

  EXPECT_EQ(initiators_indexes_[6], 0);
  EXPECT_EQ(responders_indexes_[6], 1);
  EXPECT_EQ(initiators_indexes_[7], 0);
  EXPECT_EQ(responders_indexes_[7], 2);
  EXPECT_EQ(initiators_indexes_[8], 1);
  EXPECT_EQ(responders_indexes_[8], 1);
  EXPECT_EQ(initiators_indexes_[9], 1);
  EXPECT_EQ(responders_indexes_[9], 2);
  EXPECT_EQ(initiators_indexes_[10], 0);
  EXPECT_EQ(responders_indexes_[10], 1);

  EXPECT_EQ(initiators_indexes_[34], 0);
  EXPECT_EQ(responders_indexes_[34], 0);
  EXPECT_EQ(initiators_indexes_[35], 0);
  EXPECT_EQ(responders_indexes_[35], 1);
  EXPECT_EQ(initiators_indexes_[36], 0);
  EXPECT_EQ(responders_indexes_[36], 2);
  EXPECT_EQ(initiators_indexes_[37], 1);
  EXPECT_EQ(responders_indexes_[37], 0);
  EXPECT_EQ(initiators_indexes_[38], 1);
  EXPECT_EQ(responders_indexes_[38], 1);
  EXPECT_EQ(initiators_indexes_[39], 1);
  EXPECT_EQ(responders_indexes_[39], 2);
  EXPECT_EQ(initiators_indexes_[40], 0);
  EXPECT_EQ(responders_indexes_[40], 0);

  EXPECT_EQ(initiators_indexes_.size(), 60);
  EXPECT_EQ(responders_indexes_.size(), 60);
}

TEST_F(TestPositionAwareScheduler, checkPollWhenOnlyOneResponderIsVisible) {
  init(30, 20);
  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(100));
  scheduler_->update_robot_position(Eigen::Vector3d(23, 0, 0));
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(1900));
  scheduler_->stop();

  EXPECT_EQ(initiators_indexes_[6], 0);
  EXPECT_EQ(responders_indexes_[6], 0);
  EXPECT_EQ(initiators_indexes_[7], 0);
  EXPECT_EQ(responders_indexes_[7], 1);
  EXPECT_EQ(initiators_indexes_[8], 0);
  EXPECT_EQ(responders_indexes_[8], 2);
  EXPECT_EQ(initiators_indexes_[9], 1);
  EXPECT_EQ(responders_indexes_[9], 0);
  EXPECT_EQ(initiators_indexes_[10], 1);
  EXPECT_EQ(responders_indexes_[10], 1);
  EXPECT_EQ(initiators_indexes_[11], 1);
  EXPECT_EQ(responders_indexes_[11], 2);
  EXPECT_EQ(initiators_indexes_[12], 0);
  EXPECT_EQ(responders_indexes_[12], 0);

  EXPECT_EQ(initiators_indexes_.size(), 33);
  EXPECT_EQ(responders_indexes_.size(), 33);
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
