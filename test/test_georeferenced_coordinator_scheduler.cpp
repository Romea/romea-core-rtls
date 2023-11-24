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

#include <string>
#include <vector>
#include <map>

// gtest
#include "gtest/gtest.h"

// romea
#include "romea_core_rtls/coordination/RTLSGeoreferencedCoordinatorScheduler.hpp"

class TestGeoreferencedCoordinatorScheduler : public ::testing::Test
{
protected:
  TestGeoreferencedCoordinatorScheduler()
  : scheduler_(nullptr),
    initiatorsIndexes_(),
    respondersIndexes_()
  {
  }

  void init(
    const double & pollRate,
    const double & maximalResearchDistance)
  {
    std::vector<std::string> initiatorsNames{"initiator0", "initiator1"};

    romea::core::VectorOfEigenVector3d initiatorsPositions = {
      Eigen::Vector3d(1.0, 0.5, 2.0),
      Eigen::Vector3d(-1.0, -0.5, 2.0)};

    std::vector<std::string> respondersNames({"responder0", "responder1", "responder2"});

    romea::core::VectorOfEigenVector3d respondersPositions = {
      Eigen::Vector3d(-10.0, 0.0, 2.0),
      Eigen::Vector3d(0.0, 0.0, 2.0),
      Eigen::Vector3d(10.0, 0.0, 2.0)
    };

    auto callback = [this](
      const size_t & initiatorIndex,
      const size_t & responderIndex,
      const romea::core::Duration & /*timeout*/)
      {
        initiatorsIndexes_.push_back(initiatorIndex);
        respondersIndexes_.push_back(responderIndex);
        return romea::core::RTLSTransceiverRangingResult();
      };

    scheduler_ = std::make_unique<romea::core::RTLSGeoreferencedCoordinatorScheduler>(
      pollRate, maximalResearchDistance,
      initiatorsNames, initiatorsPositions,
      respondersNames, respondersPositions,
      callback);
  }

  std::unique_ptr<romea::core::RTLSGeoreferencedCoordinatorScheduler> scheduler_;
  std::vector<size_t> initiatorsIndexes_;
  std::vector<size_t> respondersIndexes_;
};


TEST_F(TestGeoreferencedCoordinatorScheduler, checkPollWhenNoPositionIsGiven)
{
  init(30, 20);

  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromSecond(2));
  scheduler_->stop();

  EXPECT_EQ(initiatorsIndexes_[6], 0);
  EXPECT_EQ(respondersIndexes_[6], 0);
  EXPECT_EQ(initiatorsIndexes_[7], 0);
  EXPECT_EQ(respondersIndexes_[7], 1);
  EXPECT_EQ(initiatorsIndexes_[8], 0);
  EXPECT_EQ(respondersIndexes_[8], 2);
  EXPECT_EQ(initiatorsIndexes_[9], 1);
  EXPECT_EQ(respondersIndexes_[9], 0);
  EXPECT_EQ(initiatorsIndexes_[10], 1);
  EXPECT_EQ(respondersIndexes_[10], 1);
  EXPECT_EQ(initiatorsIndexes_[11], 1);
  EXPECT_EQ(respondersIndexes_[11], 2);
  EXPECT_EQ(initiatorsIndexes_[12], 0);
  EXPECT_EQ(respondersIndexes_[12], 0);
  EXPECT_EQ(initiatorsIndexes_.size(), 60);
  EXPECT_EQ(respondersIndexes_.size(), 60);
}

TEST_F(TestGeoreferencedCoordinatorScheduler, checkPollWhenAllRespondersAreVisible)
{
  init(30, 20);
  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(100));
  scheduler_->updateRobotPosition(Eigen::Vector3d::Zero());
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(1900));
  scheduler_->stop();

  EXPECT_EQ(initiatorsIndexes_[6], 0);
  EXPECT_EQ(respondersIndexes_[6], 0);
  EXPECT_EQ(initiatorsIndexes_[7], 0);
  EXPECT_EQ(respondersIndexes_[7], 1);
  EXPECT_EQ(initiatorsIndexes_[8], 0);
  EXPECT_EQ(respondersIndexes_[8], 2);
  EXPECT_EQ(initiatorsIndexes_[9], 1);
  EXPECT_EQ(respondersIndexes_[9], 0);
  EXPECT_EQ(initiatorsIndexes_[10], 1);
  EXPECT_EQ(respondersIndexes_[10], 1);
  EXPECT_EQ(initiatorsIndexes_[11], 1);
  EXPECT_EQ(respondersIndexes_[11], 2);
  EXPECT_EQ(initiatorsIndexes_[12], 0);
  EXPECT_EQ(respondersIndexes_[12], 0);
  EXPECT_EQ(initiatorsIndexes_.size(), 60);
  EXPECT_EQ(respondersIndexes_.size(), 60);
}

TEST_F(TestGeoreferencedCoordinatorScheduler, checkPollWhenTwoRespondersAreVisible)
{
  init(30, 20);
  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(100));
  scheduler_->updateRobotPosition(Eigen::Vector3d(13, 0, 0));
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(1900));
  scheduler_->stop();


  EXPECT_EQ(initiatorsIndexes_[6], 0);
  EXPECT_EQ(respondersIndexes_[6], 1);
  EXPECT_EQ(initiatorsIndexes_[7], 0);
  EXPECT_EQ(respondersIndexes_[7], 2);
  EXPECT_EQ(initiatorsIndexes_[8], 1);
  EXPECT_EQ(respondersIndexes_[8], 1);
  EXPECT_EQ(initiatorsIndexes_[9], 1);
  EXPECT_EQ(respondersIndexes_[9], 2);
  EXPECT_EQ(initiatorsIndexes_[10], 0);
  EXPECT_EQ(respondersIndexes_[10], 1);

  EXPECT_EQ(initiatorsIndexes_[34], 0);
  EXPECT_EQ(respondersIndexes_[34], 0);
  EXPECT_EQ(initiatorsIndexes_[35], 0);
  EXPECT_EQ(respondersIndexes_[35], 1);
  EXPECT_EQ(initiatorsIndexes_[36], 0);
  EXPECT_EQ(respondersIndexes_[36], 2);
  EXPECT_EQ(initiatorsIndexes_[37], 1);
  EXPECT_EQ(respondersIndexes_[37], 0);
  EXPECT_EQ(initiatorsIndexes_[38], 1);
  EXPECT_EQ(respondersIndexes_[38], 1);
  EXPECT_EQ(initiatorsIndexes_[39], 1);
  EXPECT_EQ(respondersIndexes_[39], 2);
  EXPECT_EQ(initiatorsIndexes_[40], 0);
  EXPECT_EQ(respondersIndexes_[40], 0);

  EXPECT_EQ(initiatorsIndexes_.size(), 60);
  EXPECT_EQ(respondersIndexes_.size(), 60);
}

TEST_F(TestGeoreferencedCoordinatorScheduler, checkPollWhenOnlyOneResponderIsVisible)
{
  init(30, 20);
  scheduler_->start();
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(100));
  scheduler_->updateRobotPosition(Eigen::Vector3d(23, 0, 0));
  std::this_thread::sleep_for(romea::core::durationFromMilliSecond(1900));
  scheduler_->stop();


  EXPECT_EQ(initiatorsIndexes_[6], 0);
  EXPECT_EQ(respondersIndexes_[6], 0);
  EXPECT_EQ(initiatorsIndexes_[7], 0);
  EXPECT_EQ(respondersIndexes_[7], 1);
  EXPECT_EQ(initiatorsIndexes_[8], 0);
  EXPECT_EQ(respondersIndexes_[8], 2);
  EXPECT_EQ(initiatorsIndexes_[9], 1);
  EXPECT_EQ(respondersIndexes_[9], 0);
  EXPECT_EQ(initiatorsIndexes_[10], 1);
  EXPECT_EQ(respondersIndexes_[10], 1);
  EXPECT_EQ(initiatorsIndexes_[11], 1);
  EXPECT_EQ(respondersIndexes_[11], 2);
  EXPECT_EQ(initiatorsIndexes_[12], 0);
  EXPECT_EQ(respondersIndexes_[12], 0);

  EXPECT_EQ(initiatorsIndexes_.size(), 33);
  EXPECT_EQ(respondersIndexes_.size(), 33);
}


//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
