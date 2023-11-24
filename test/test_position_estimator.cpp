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
#include "romea_core_common/containers/Eigen/VectorOfEigenVector.hpp"
#include "romea_core_rtls/trilateration/RTLSPosition2DEstimator.hpp"


//-----------------------------------------------------------------------------
TEST(TestRtlsPositionEstimator, testPositionEstimatorWithTwoAnchors)
{
  auto tag0Position = Eigen::Vector3d(5.0, 2.0, 2.0);
  auto anchor0Position = Eigen::Vector3d(0.0, 0.3, 1);
  auto anchor1Position = Eigen::Vector3d(0.0, -0.3, 1);
  double r00 = (tag0Position - anchor0Position).head<2>().norm();
  double r01 = (tag0Position - anchor1Position).head<2>().norm();

  romea::core::RTLSPosition2DEstimator estimator(
    {anchor0Position, anchor1Position}, 0.001);
  EXPECT_TRUE(estimator.init({r00, r01}));
  EXPECT_TRUE(estimator.estimate(20, 0.02));

  auto tag0EstimatedPosition = estimator.getEstimate();

  EXPECT_NEAR(tag0Position.x(), tag0EstimatedPosition.x(), 0.001);
  EXPECT_NEAR(tag0Position.y(), tag0EstimatedPosition.y(), 0.001);
}

//-----------------------------------------------------------------------------
TEST(TestRtlsPositionEstimator, testPositionEstimatorWithThreeAnchorsUp)
{
  auto tag0Position = Eigen::Vector3d(-4, 6, 1);
  auto anchor0Position = Eigen::Vector3d(0, 0.6, 2);
  auto anchor1Position = Eigen::Vector3d(0, -0.6, 1.5);
  auto anchor2Position = Eigen::Vector3d(1, 0, 1.8);
  double r00 = (tag0Position - anchor0Position).head<2>().norm();
  double r01 = (tag0Position - anchor1Position).head<2>().norm();
  double r02 = (tag0Position - anchor2Position).head<2>().norm();


  romea::core::RTLSPosition2DEstimator estimator(
    {anchor0Position, anchor1Position, anchor2Position}, 0.001);

  EXPECT_TRUE(estimator.init({r00, r01, r02}));
  EXPECT_TRUE(estimator.estimate(20, 0.02));

  auto tag0EstimatedPosition = estimator.getEstimate();
  EXPECT_NEAR(tag0Position.x(), tag0EstimatedPosition.x(), 0.001);
  EXPECT_NEAR(tag0Position.y(), tag0EstimatedPosition.y(), 0.001);
}

//-----------------------------------------------------------------------------
TEST(TestRtlsPositionEstimator, testPositionEstimatorWithThreeAnchorsDown)
{
  auto tag0Position = Eigen::Vector3d(-6, -7, 1);
  auto anchor0Position = Eigen::Vector3d(0, 0.6, 2);
  auto anchor1Position = Eigen::Vector3d(0, -0.6, 1.5);
  auto anchor2Position = Eigen::Vector3d(-2, 0, 1.8);
  double r00 = (tag0Position - anchor0Position).head<2>().norm();
  double r01 = (tag0Position - anchor1Position).head<2>().norm();
  double r02 = (tag0Position - anchor2Position).head<2>().norm();

  romea::core::RTLSPosition2DEstimator estimator(
    {anchor0Position, anchor1Position, anchor2Position}, 0.001);
  EXPECT_TRUE(estimator.init({r00, r01, r02}));
  EXPECT_TRUE(estimator.estimate(20, 0.02));

  auto tag0EstimatedPosition = estimator.getEstimate();
  EXPECT_NEAR(tag0Position.x(), tag0EstimatedPosition.x(), 0.001);
  EXPECT_NEAR(tag0Position.y(), tag0EstimatedPosition.y(), 0.001);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
