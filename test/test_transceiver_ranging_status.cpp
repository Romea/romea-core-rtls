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


// gtest
#include "gtest/gtest.h"

// romea
#include "romea_core_rtls/transceiver/RTLSTransceiverRangingStatus.hpp"

class TestTransceiverRangingStatus : public ::testing::Test
{
protected:
  TestTransceiverRangingStatus()
  : minimalRange_(0.5),
    maximalRange_(20),
    rxPowerRejectionThreshold_(20)
  {}

  // static void SetUpTestCase()
  // {
  //   rclcpp::init(0, nullptr);
  // }

  // static void TearDownTestCase()
  // {
  //   rclcpp::shutdown();
  // }

  // void SetUp() override
  // {
  //   std::string xacro_file = std::string(TEST_DIR) + "/test_hardware_interface1FAS2RWD.xacro";
  //   std::string urdf_file = "/tmp/test_hardware_interface1FAS2RWD.urdf";
  //   std::string cmd = "xacro " + xacro_file + " > " + urdf_file;
  //   std::system(cmd.c_str());

  //   std::ifstream file(urdf_file.c_str());
  //   std::stringstream buffer;
  //   buffer << file.rdbuf();
  //   //    std::cout << buffer.str() <<std::endl;

  //   info = hardware_interface::parse_control_resources_from_urdf(buffer.str());
  // }

  romea::RTLSTransceiverRangingStatus getRangingStatus(
    const romea::RTLSTransceiverRangingResult & result)
  {
    return romea::getRangingStatus(
      result, minimalRange_, maximalRange_,
      rxPowerRejectionThreshold_);
  }

  double minimalRange_;
  double maximalRange_;
  uint8_t rxPowerRejectionThreshold_;
};


TEST_F(TestTransceiverRangingStatus, checkRangingIsFailed)
{
  romea::RTLSTransceiverRangingResult result;
  result.range = 0.;
  result.firstPathRxPowerLevel = 0;
  result.totalRxPowerLevel = 0;

  EXPECT_TRUE(getRangingStatus(result) == romea::RTLSTransceiverRangingStatus::FAILED);
}

TEST_F(TestTransceiverRangingStatus, checkRangingIsUnavailableWhenRangeIsLowerThanMinimalRange)
{
  romea::RTLSTransceiverRangingResult result;
  result.range = 0.2;
  result.firstPathRxPowerLevel = 8;
  result.totalRxPowerLevel = 10;

  EXPECT_TRUE(getRangingStatus(result) == romea::RTLSTransceiverRangingStatus::UNAVAILABLE);
}

TEST_F(TestTransceiverRangingStatus, checkRangingIsUnavailableWhenRangeIsHigherThanMaximalRange)
{
  romea::RTLSTransceiverRangingResult result;
  result.range = 22.;
  result.firstPathRxPowerLevel = 8;
  result.totalRxPowerLevel = 10;
  EXPECT_TRUE(getRangingStatus(result) == romea::RTLSTransceiverRangingStatus::UNAVAILABLE);
}

TEST_F(TestTransceiverRangingStatus, checkRangingIsUnavailableWhenFirstPathRxPowerLevelIsTooLow)
{
  romea::RTLSTransceiverRangingResult result;
  result.range = 10.;
  result.firstPathRxPowerLevel = 5;
  result.totalRxPowerLevel = 30;
  EXPECT_TRUE(getRangingStatus(result) == romea::RTLSTransceiverRangingStatus::UNAVAILABLE);
}

TEST_F(TestTransceiverRangingStatus, checkIsAvailable)
{
  romea::RTLSTransceiverRangingResult result;
  result.range = 10.;
  result.firstPathRxPowerLevel = 28;
  result.totalRxPowerLevel = 30;
  EXPECT_TRUE(getRangingStatus(result) == romea::RTLSTransceiverRangingStatus::AVAILABLE);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
