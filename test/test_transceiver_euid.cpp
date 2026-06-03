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

// gtest
#include "gtest/gtest.h"

// std
#include <set>

// romea
#include "romea_core_rtls/transceiver.hpp"

//-----------------------------------------------------------------------------
TEST(TestTransceiverEUID, checkEqualOperator) {
  romea::core::RTLSTransceiver::EUID euid1{1, 1};
  romea::core::RTLSTransceiver::EUID euid2{1, 2};
  romea::core::RTLSTransceiver::EUID euid3{2, 1};

  EXPECT_TRUE(euid1 == euid1);
  EXPECT_FALSE(euid1 == euid2);
  EXPECT_FALSE(euid1 == euid3);
}

//-----------------------------------------------------------------------------
TEST(TestTransceiverEUID, checkLowerThanOperator) {
  romea::core::RTLSTransceiver::EUID euid00{0, 0};
  romea::core::RTLSTransceiver::EUID euid01{0, 1};
  romea::core::RTLSTransceiver::EUID euid10{1, 0};
  romea::core::RTLSTransceiver::EUID euid11{1, 1};

  EXPECT_TRUE(euid00 < euid01);
  EXPECT_TRUE(euid01 < euid10);
  EXPECT_TRUE(euid10 < euid11);
}

//-----------------------------------------------------------------------------
TEST(TestTransceiverEUID, checkLowerThanOperatorOnBoundaryValues) {
  romea::core::RTLSTransceiver::EUID euid0Max{0, 65535};
  romea::core::RTLSTransceiver::EUID euid10{1, 0};

  EXPECT_TRUE(euid0Max < euid10);
  EXPECT_FALSE(euid10 < euid0Max);
}

//-----------------------------------------------------------------------------
TEST(TestTransceiverEUID, checkEUIDCanBeUsedInOrderedContainers) {
  std::set<romea::core::RTLSTransceiver::EUID> euids;

  euids.insert({0, 65535});
  euids.insert({1, 0});
  euids.insert({1, 0});

  EXPECT_EQ(euids.size(), 2);
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
