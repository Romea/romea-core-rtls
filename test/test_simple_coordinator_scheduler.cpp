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
#include <string>
#include <vector>
#include <memory>
#include <map>

// gtest
#include "gtest/gtest.h"

// romea
#include "romea_core_rtls/coordination/RTLSSimpleCoordinatorScheduler.hpp"

class TestSimpleCoordinatorScheduler : public ::testing::Test
{
protected:
  TestSimpleCoordinatorScheduler()
  : scheduler_(nullptr),
    initiatorsIndexes_(),
    respondersIndexes_()
  {
  }

  void init(
    const double & pollRate,
    const std::vector<std::string> & initiatorsNames,
    const std::vector<std::string> & respondersNames)
  {
    auto callback = [this](
      const size_t & initiatorIndex,
      const size_t & responderIndex,
      const romea::core::Duration & /*timeout*/)
      {
        initiatorsIndexes_.push_back(initiatorIndex);
        respondersIndexes_.push_back(responderIndex);
        return romea::core::RTLSTransceiverRangingResult();
      };

    scheduler_ = std::make_unique<romea::core::RTLSSimpleCoordinatorScheduler>(
      pollRate, initiatorsNames, respondersNames, callback);
  }

  std::unique_ptr<romea::core::RTLSSimpleCoordinatorScheduler> scheduler_;
  std::vector<size_t> initiatorsIndexes_;
  std::vector<size_t> respondersIndexes_;
};


TEST_F(TestSimpleCoordinatorScheduler, checkPollWhenOneResponderIsUsed)
{
  init(20.0, {"initiator0", "initiator1"}, {"responder0"});

  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromSecond(1));
  scheduler_->stop();

  EXPECT_EQ(initiatorsIndexes_[0], 0);
  EXPECT_EQ(respondersIndexes_[0], 0);
  EXPECT_EQ(initiatorsIndexes_[1], 1);
  EXPECT_EQ(respondersIndexes_[1], 0);
  EXPECT_EQ(initiatorsIndexes_[2], 0);
  EXPECT_EQ(respondersIndexes_[2], 0);
  EXPECT_EQ(initiatorsIndexes_.size(), 20);
  EXPECT_EQ(respondersIndexes_.size(), 20);

  auto report = scheduler_->getReport();
  EXPECT_STREQ(report.info["initiator0"].c_str(), "");
  EXPECT_STREQ(report.info["initiator1"].c_str(), "");
  EXPECT_STREQ(report.info["responder0"].c_str(), "");
}

TEST_F(TestSimpleCoordinatorScheduler, checkPollWhenTwoRespondersIsUsed)
{
  init(20.0, {"initiator0", "initiator1"}, {"responder0", "responder1"});

  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromSecond(1));
  scheduler_->stop();

  EXPECT_EQ(initiatorsIndexes_[0], 0);
  EXPECT_EQ(respondersIndexes_[0], 0);
  EXPECT_EQ(initiatorsIndexes_[1], 0);
  EXPECT_EQ(respondersIndexes_[1], 1);
  EXPECT_EQ(initiatorsIndexes_[2], 1);
  EXPECT_EQ(respondersIndexes_[2], 0);
  EXPECT_EQ(initiatorsIndexes_[3], 1);
  EXPECT_EQ(respondersIndexes_[3], 1);
  EXPECT_EQ(initiatorsIndexes_[4], 0);
  EXPECT_EQ(respondersIndexes_[4], 0);
  EXPECT_EQ(initiatorsIndexes_.size(), 20);
  EXPECT_EQ(respondersIndexes_.size(), 20);

  auto report = scheduler_->getReport();
  EXPECT_STREQ(report.info["initiator0"].c_str(), "");
  EXPECT_STREQ(report.info["initiator1"].c_str(), "");
  EXPECT_STREQ(report.info["responder0"].c_str(), "");
  EXPECT_STREQ(report.info["responder1"].c_str(), "");
}


//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
