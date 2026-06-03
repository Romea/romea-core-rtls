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
#include <thread>
#include <vector>

// gtest
#include "gtest/gtest.h"

// romea
#include "romea_core_rtls/scheduling/round_robin_scheduler.hpp"

class TestRoundRobinScheduler : public ::testing::Test {
 protected:
  void init(const double& poll_rate,
            const std::vector<std::string>& initiators_names,
            const std::vector<std::string>& responders_names) {
    poll_rate_ = poll_rate;
    initiators_names_ = initiators_names;
    responders_names_ = responders_names;

    auto callback = [this](const size_t& initiator_index,
                           const size_t& responder_index,
                           const romea::core::Duration& timeout) {
      initiators_indexes_.push_back(initiator_index);
      responders_indexes_.push_back(responder_index);
      timeouts_.push_back(timeout);
    };

    scheduler_ = std::make_unique<romea::core::RTLSRoundRobinScheduler>(
        poll_rate, initiators_names, responders_names, callback);
  }

  void run_for_one_second() {
    scheduler_->start();
    std::this_thread::sleep_for(romea::core::durationFromSecond(1));
    scheduler_->stop();
  }

  void expect_poll_sequence(
      const std::vector<size_t>& expected_initiators_indexes,
      const std::vector<size_t>& expected_responders_indexes) {
    ASSERT_EQ(expected_initiators_indexes.size(),
              expected_responders_indexes.size());
    ASSERT_GE(initiators_indexes_.size(), expected_initiators_indexes.size());
    ASSERT_GE(responders_indexes_.size(), expected_responders_indexes.size());

    for (size_t i = 0; i < expected_initiators_indexes.size(); ++i) {
      EXPECT_EQ(initiators_indexes_[i], expected_initiators_indexes[i])
          << "Unexpected initiator at poll " << i;
      EXPECT_EQ(responders_indexes_[i], expected_responders_indexes[i])
          << "Unexpected responder at poll " << i;
    }
  }

  void expect_poll_count(const size_t& expected_count) {
    EXPECT_EQ(initiators_indexes_.size(), expected_count);
    EXPECT_EQ(responders_indexes_.size(), expected_count);
    EXPECT_EQ(timeouts_.size(), expected_count);
  }

  void expect_timeout_shorter_than_poll_period() {
    ASSERT_FALSE(timeouts_.empty());

    const double poll_period = 1.0 / poll_rate_;
    const double timeout = romea::core::durationToSecond(timeouts_[0]);
    EXPECT_NEAR(timeout, poll_period - 0.001, 1e-9);
  }

  void expect_empty_diagnostic_report() {
    auto report = scheduler_->get_report();

    for (const auto& name : initiators_names_) {
      EXPECT_STREQ(report.info[name].c_str(), "");
    }

    for (const auto& name : responders_names_) {
      EXPECT_STREQ(report.info[name].c_str(), "");
    }
  }

 protected:
  double poll_rate_;
  std::vector<std::string> initiators_names_;
  std::vector<std::string> responders_names_;
  std::unique_ptr<romea::core::RTLSRoundRobinScheduler> scheduler_;
  std::vector<size_t> initiators_indexes_;
  std::vector<size_t> responders_indexes_;
  std::vector<romea::core::Duration> timeouts_;
};

//-----------------------------------------------------------------------------
TEST_F(TestRoundRobinScheduler, pollsOneResponderWithAllInitiators) {
  init(20.0, {"initiator0", "initiator1"}, {"responder0"});

  run_for_one_second();

  expect_poll_sequence({0, 1, 0}, {0, 0, 0});
  expect_poll_count(20);
  expect_timeout_shorter_than_poll_period();
  expect_empty_diagnostic_report();
}

//-----------------------------------------------------------------------------
TEST_F(TestRoundRobinScheduler, pollsAllInitiatorResponderPairs) {
  init(20.0, {"initiator0", "initiator1"}, {"responder0", "responder1"});

  run_for_one_second();

  expect_poll_sequence({0, 0, 1, 1, 0}, {0, 1, 0, 1, 0});
  expect_poll_count(20);
  expect_timeout_shorter_than_poll_period();
  expect_empty_diagnostic_report();
}

//-----------------------------------------------------------------------------
TEST_F(TestRoundRobinScheduler, rejectsEmptyInitiatorList) {
  EXPECT_ANY_THROW(init(20.0, {}, {"responder0"}));
}

//-----------------------------------------------------------------------------
TEST_F(TestRoundRobinScheduler, rejectsEmptyResponderList) {
  EXPECT_ANY_THROW(init(20.0, {"initiator0"}, {}));
}

//-----------------------------------------------------------------------------
TEST_F(TestRoundRobinScheduler, rejectsInvalidPollRate) {
  EXPECT_ANY_THROW(init(0.0, {"initiator0"}, {"responder0"}));
  EXPECT_ANY_THROW(init(-1.0, {"initiator0"}, {"responder0"}));
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
