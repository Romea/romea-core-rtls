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
#include <vector>

// gtest
#include "gtest/gtest.h"

// romea
#include "romea_core_common/containers/Eigen/VectorOfEigenVector.hpp"
#include "romea_core_rtls/trilateration/RTLSSimpleTrilateration2D.hpp"


//-----------------------------------------------------------------------------
TEST(TestRobotToHuman, testTrilateration2D2A)
{
  Eigen::Vector2d tagPosition(5, 2);

  romea::VectorOfEigenVector2d anchorPositions(2);
  anchorPositions[0] = Eigen::Vector2d(0, 0.3);
  anchorPositions[1] = Eigen::Vector2d(0, -0.3);

  std::vector<double> ranges(2);
  ranges[0] = (tagPosition - anchorPositions[0]).norm();
  ranges[1] = (tagPosition - anchorPositions[1]).norm();

  Eigen::Vector2d tagEstimatedPosition =
    romea::SimpleTrilateration2D::compute(anchorPositions, ranges);
  EXPECT_NEAR(tagPosition.x(), tagEstimatedPosition.x(), 0.001);
  EXPECT_NEAR(tagPosition.y(), tagEstimatedPosition.y(), 0.001);
}


//-----------------------------------------------------------------------------
TEST(TestRobotToHuman, testTrilateration2D3AUp)
{
  Eigen::Vector2d tagPosition = Eigen::Vector2d(6, -3);

  romea::VectorOfEigenVector2d anchorPositions(3);
  anchorPositions[0] = Eigen::Vector2d(0, 0.6);
  anchorPositions[1] = Eigen::Vector2d(0, -0.6);
  anchorPositions[2] = Eigen::Vector2d(1, 0);

  std::vector<double> ranges(3);
  ranges[0] = (tagPosition - anchorPositions[0]).norm();
  ranges[1] = (tagPosition - anchorPositions[1]).norm();
  ranges[2] = (tagPosition - anchorPositions[2]).norm();

  Eigen::Vector2d tagEstimatedPosition =
    romea::SimpleTrilateration2D::compute(anchorPositions, ranges);

  EXPECT_NEAR(tagPosition.x(), tagEstimatedPosition.x(), 0.001);
  EXPECT_NEAR(tagPosition.y(), tagEstimatedPosition.y(), 0.001);
}


//-----------------------------------------------------------------------------
TEST(TestRobotToHuman, testTrilateration2D3ADown)
{
  Eigen::Vector2d tagPosition = Eigen::Vector2d(6, -3);

  romea::VectorOfEigenVector2d anchorPositions(3);
  anchorPositions[0] = Eigen::Vector2d(0, 0.7),
  anchorPositions[1] = Eigen::Vector2d(0, -0.5),
  anchorPositions[2] = Eigen::Vector2d(-0.7, 0);

  std::vector<double> ranges(3);
  ranges[0] = (tagPosition - anchorPositions[0]).norm();
  ranges[1] = (tagPosition - anchorPositions[1]).norm();
  ranges[2] = (tagPosition - anchorPositions[2]).norm();

  Eigen::Vector2d tagEstimatedPosition =
    romea::SimpleTrilateration2D::compute(anchorPositions, ranges);

  EXPECT_NEAR(tagPosition.x(), tagEstimatedPosition.x(), 0.001);
  EXPECT_NEAR(tagPosition.y(), tagEstimatedPosition.y(), 0.001);
}


//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
