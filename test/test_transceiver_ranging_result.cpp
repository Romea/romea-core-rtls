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
#include "romea_core_rtls/transceiver/RTLSTransceiverRangingResult.hpp"


TEST(TestTransceiverRangingResult, checkEmptyResult)
{
  romea::RTLSTransceiverRangingResult result;
  result.range = 0.;
  result.firstPathRxPowerLevel = 0;
  result.totalRxPowerLevel = 0;

  EXPECT_TRUE(romea::isEmpty(result));
}

TEST(TestTransceiverRangingResult, testNotEmptyResult)
{
  romea::RTLSTransceiverRangingResult result;
  result.range = 10.0;
  result.firstPathRxPowerLevel = 10;
  result.totalRxPowerLevel = 20;
  EXPECT_FALSE(romea::isEmpty(result));
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
