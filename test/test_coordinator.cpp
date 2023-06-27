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
#include "romea_core_rtls/coordination/RTLSCoordinator.hpp"

const double pollRate = 10.;
const double minimalRange = 0.5;
const double maximalRange = 20.;
const uint8_t fakeTotalRxPowerLevel = 30;
const uint8_t fakeFirstPathRxPowerLevel = 22;
const uint8_t rxPowerRejectionThreshold = 20;
const std::vector<std::string> initiatorNames = {"initiator0", "initiator1"};
const std::vector<Eigen::Vector3d> initiatorPositions =
{Eigen::Vector3d(1.0, 0.5, 1.5), Eigen::Vector3d(-1.0, -0.5, 2.0)};

class TestCoordinator : public ::testing::Test
{
protected:
  TestCoordinator()
  : coordinator_(
      initiatorNames,
      initiatorPositions,
      pollRate,
      minimalRange,
      maximalRange,
      rxPowerRejectionThreshold)
  {
    for (size_t n = 0; n < initiatorNames.size(); ++n) {
      initiatorPositionsMap_[initiatorNames[n]] = initiatorPositions[n];
    }
  }

  void setRespondersInfo(
    const std::vector<std::string> & responderNames,
    const std::vector<Eigen::Vector3d> & responderPositions)
  {
    for (size_t n = 0; n < responderPositions.size(); ++n) {
      responderPositionsMap_[responderNames[n]] = responderPositions[n];
    }

    coordinator_.setRespondersInfo(responderNames, responderPositions);
  }

  romea::RTLSTransceivers pollOnce()
  {
    return coordinator_.poll().value();
  }

  std::vector<romea::RTLSTransceivers> pollSome(const size_t numberOfPolls)
  {
    std::vector<romea::RTLSTransceivers> polls(numberOfPolls);
    for (size_t n = 0; n < numberOfPolls; ++n) {
      polls[n] = pollOnce();
    }

    return polls;
  }

  romea::RTLSTransceiverRangingResult getRangingResult(const romea::RTLSTransceivers & poll)
  {
    auto initiatorPosition = initiatorPositionsMap_[poll.initiatorName];
    auto responderPosition = responderPositionsMap_[poll.responderName];

    romea::RTLSTransceiverRangingResult result;
    result.totalRxPowerLevel = fakeTotalRxPowerLevel;
    result.firstPathRxPowerLevel = fakeFirstPathRxPowerLevel;
    result.range = (responderPosition - initiatorPosition).norm();
    return result;
  }

  std::vector<romea::RTLSTransceiverRangingStatus> getRangingStatus(const size_t numberOfPolls)
  {
    std::vector<romea::RTLSTransceiverRangingStatus> status(numberOfPolls);
    for (size_t n = 0; n < numberOfPolls; ++n) {
      auto poll = pollOnce();
      status[n] = coordinator_.feedback(poll, getRangingResult(poll));
    }
    return status;
  }

  std::map<std::string, Eigen::Vector3d> initiatorPositionsMap_;
  std::map<std::string, Eigen::Vector3d> responderPositionsMap_;
  romea::RTLSCoordinator coordinator_;
};

TEST_F(TestCoordinator, checkCannotGetPoll)
{
  EXPECT_FALSE(coordinator_.poll().has_value());
}

TEST_F(TestCoordinator, checkPollWhenOneResponderIsUsed)
{
  setRespondersInfo({"responder0"}, {Eigen::Vector3d(20, 0.0, 1.0)});
  auto polls = pollSome(3);
  EXPECT_STREQ(polls[0].initiatorName.c_str(), "initiator0");
  EXPECT_STREQ(polls[0].responderName.c_str(), "responder0");
  EXPECT_STREQ(polls[1].initiatorName.c_str(), "initiator1");
  EXPECT_STREQ(polls[1].responderName.c_str(), "responder0");
  EXPECT_STREQ(polls[2].initiatorName.c_str(), "initiator0");
  EXPECT_STREQ(polls[2].responderName.c_str(), "responder0");
}

TEST_F(TestCoordinator, checkRanginStatusWhenOneResponderIsUsed)
{
  setRespondersInfo({"responder0"}, {Eigen::Vector3d(20, 0.0, 1.0)});
  auto status = getRangingStatus(2);
  EXPECT_TRUE(status[0] == romea::RTLSTransceiverRangingStatus::AVAILABLE);
  EXPECT_TRUE(status[1] == romea::RTLSTransceiverRangingStatus::UNAVAILABLE);
}


TEST_F(TestCoordinator, checkPollWhenTwoRespondersIsUsed)
{
  setRespondersInfo(
    {"responder0", "responder1"},
    {Eigen::Vector3d(20, 0.0, 1.0), Eigen::Vector3d(-20, 0.0, 1.0)});

  auto polls = pollSome(5);
  EXPECT_STREQ(polls[0].initiatorName.c_str(), "initiator0");
  EXPECT_STREQ(polls[0].responderName.c_str(), "responder0");
  EXPECT_STREQ(polls[1].initiatorName.c_str(), "initiator0");
  EXPECT_STREQ(polls[1].responderName.c_str(), "responder1");
  EXPECT_STREQ(polls[2].initiatorName.c_str(), "initiator1");
  EXPECT_STREQ(polls[2].responderName.c_str(), "responder0");
  EXPECT_STREQ(polls[3].initiatorName.c_str(), "initiator1");
  EXPECT_STREQ(polls[3].responderName.c_str(), "responder1");
  EXPECT_STREQ(polls[4].initiatorName.c_str(), "initiator0");
  EXPECT_STREQ(polls[4].responderName.c_str(), "responder0");
}

TEST_F(TestCoordinator, checkRanginStatusWhenTwoResponderIsUsed)
{
  setRespondersInfo(
    {"responder0", "responder1"},
    {Eigen::Vector3d(20, 0.0, 1.0), Eigen::Vector3d(-20, 0.0, 1.0)});

  auto status = getRangingStatus(4);
  EXPECT_TRUE(status[0] == romea::RTLSTransceiverRangingStatus::AVAILABLE);
  EXPECT_TRUE(status[1] == romea::RTLSTransceiverRangingStatus::UNAVAILABLE);
  EXPECT_TRUE(status[2] == romea::RTLSTransceiverRangingStatus::UNAVAILABLE);
  EXPECT_TRUE(status[3] == romea::RTLSTransceiverRangingStatus::AVAILABLE);

}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
