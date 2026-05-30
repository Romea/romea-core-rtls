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
#include "romea_core_rtls/scheduling/round_robin_scheduler.hpp"

class TestRoundRobinScheduler : public ::testing::Test {
 protected:
  TestRoundRobinScheduler()
      : scheduler_(nullptr), initiators_indexes_(), responders_indexes_() {}

  void init(const double& pollRate,
            const std::vector<std::string>& initiators_names,
            const std::vector<std::string>& responders_names) {
    auto callback = [this](const size_t& initiator_index,
                           const size_t& responder_index,
                           const romea::core::Duration& /*timeout*/) {
      initiators_indexes_.push_back(initiator_index);
      responders_indexes_.push_back(responder_index);
      return romea::core::RTLSRangingResult();
    };

    scheduler_ = std::make_unique<romea::core::RTLSRoundRobinScheduler>(
        pollRate, initiators_names, responders_names, callback);
  }

  std::unique_ptr<romea::core::RTLSRoundRobinScheduler> scheduler_;
  std::vector<size_t> initiators_indexes_;
  std::vector<size_t> responders_indexes_;
};

TEST_F(TestRoundRobinScheduler, checkPollWhenOneResponderIsUsed) {
  init(20.0, {"initiator0", "initiator1"}, {"responder0"});

  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromSecond(1));
  scheduler_->stop();

  EXPECT_EQ(initiators_indexes_[0], 0);
  EXPECT_EQ(responders_indexes_[0], 0);
  EXPECT_EQ(initiators_indexes_[1], 1);
  EXPECT_EQ(responders_indexes_[1], 0);
  EXPECT_EQ(initiators_indexes_[2], 0);
  EXPECT_EQ(responders_indexes_[2], 0);
  EXPECT_EQ(initiators_indexes_.size(), 20);
  EXPECT_EQ(responders_indexes_.size(), 20);

  auto report = scheduler_->get_report();
  EXPECT_STREQ(report.info["initiator0"].c_str(), "");
  EXPECT_STREQ(report.info["initiator1"].c_str(), "");
  EXPECT_STREQ(report.info["responder0"].c_str(), "");
}

TEST_F(TestRoundRobinScheduler, checkPollWhenTwoRespondersIsUsed) {
  init(20.0, {"initiator0", "initiator1"}, {"responder0", "responder1"});

  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromSecond(1));
  scheduler_->stop();

  EXPECT_EQ(initiators_indexes_[0], 0);
  EXPECT_EQ(responders_indexes_[0], 0);
  EXPECT_EQ(initiators_indexes_[1], 0);
  EXPECT_EQ(responders_indexes_[1], 1);
  EXPECT_EQ(initiators_indexes_[2], 1);
  EXPECT_EQ(responders_indexes_[2], 0);
  EXPECT_EQ(initiators_indexes_[3], 1);
  EXPECT_EQ(responders_indexes_[3], 1);
  EXPECT_EQ(initiators_indexes_[4], 0);
  EXPECT_EQ(responders_indexes_[4], 0);
  EXPECT_EQ(initiators_indexes_.size(), 20);
  EXPECT_EQ(responders_indexes_.size(), 20);

  auto report = scheduler_->get_report();
  EXPECT_STREQ(report.info["initiator0"].c_str(), "");
  EXPECT_STREQ(report.info["initiator1"].c_str(), "");
  EXPECT_STREQ(report.info["responder0"].c_str(), "");
  EXPECT_STREQ(report.info["responder1"].c_str(), "");
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
