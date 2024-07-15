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
#include <algorithm>
#include <vector>
#include <cmath>

// gtest
#include "gtest/gtest.h"

// romea
#include "romea_core_common/math/EulerAngles.hpp"
#include "romea_core_rtls/serialization/Pose2DSerialization.hpp"

void checkCartesianCoordinateSerialization(const double & value)
{
  double deserializedValue;
  std::vector<unsigned char> buffer(4);
  romea::core::serializeCartesianCoordinate(value, buffer.data());
  romea::core::deserializeCartesianCoordinate(buffer.data(), deserializedValue);
  EXPECT_NEAR(value, deserializedValue, 0.001);
}

void checkOrientationSerialization(const double & value)
{
  double deserializedValue;
  std::vector<unsigned char> buffer(2);
  romea::core::serialiazeOrientation(value, buffer.data());
  romea::core::deserializeOrientation(buffer.data(), deserializedValue);
  EXPECT_LE(
    std::abs(romea::core::betweenMinusPiAndPi(deserializedValue - value)), 0.01 * M_PI / 180.);
}

void checkPositionCovarianceSerialization(const Eigen::Matrix2d & covariance)
{
  std::vector<unsigned char> buffer(1);
  Eigen::Matrix2d deserializePositionCovariance;
  romea::core::serializePositionCovariance(covariance, buffer.data());
  romea::core::deserializePositionCovariance(buffer.data(), deserializePositionCovariance);
  auto maxStd = std::max(std::sqrt(covariance(0, 0)), std::sqrt(covariance(1, 1)));
  EXPECT_NEAR(std::sqrt(deserializePositionCovariance(0, 0)), maxStd, 0.01);
  EXPECT_NEAR(std::sqrt(deserializePositionCovariance(1, 1)), maxStd, 0.01);
  EXPECT_DOUBLE_EQ(deserializePositionCovariance(0, 1), 0);
  EXPECT_DOUBLE_EQ(deserializePositionCovariance(1, 0), 0);
}

void checkOrientationVarianceSerialization(const double & value)
{
  double deserializedValue;
  std::vector<unsigned char> buffer(1);
  romea::core::serializeOrientationVariance(value, buffer.data());
  romea::core::deserializeOrientationVariance(buffer.data(), deserializedValue);
  EXPECT_LE(
    std::abs(romea::core::betweenMinusPiAndPi(deserializedValue - value)), 0.1 * M_PI / 180.);
}

//-----------------------------------------------------------------------------
TEST(TestSerialization, testCartesianCoordinateSerialization)
{
  checkCartesianCoordinateSerialization(1000000);
  checkCartesianCoordinateSerialization(10.012345);
  checkCartesianCoordinateSerialization(-987654.012345);
  checkCartesianCoordinateSerialization(-1000000);
  EXPECT_ANY_THROW(checkCartesianCoordinateSerialization(1000000.1));
}

//-----------------------------------------------------------------------------
TEST(TestSerialization, testCartesianPositionSerialization)
{
  std::vector<unsigned char> buffer(8);
  Eigen::Vector2d deserializeCartesianPosition;
  romea::core::serializeCartesianPosition(Eigen::Vector2d(-100.2937, 320.4873), buffer.data());
  romea::core::deserializeCartesianPosition(buffer.data(), deserializeCartesianPosition);
  EXPECT_NEAR(deserializeCartesianPosition.x(), -100.2937, 0.001);
  EXPECT_NEAR(deserializeCartesianPosition.y(), 320.4873, 0.001);
}

//-----------------------------------------------------------------------------
TEST(TestSerialization, testOrientationSerialization)
{
  checkOrientationSerialization(-M_PI);
  checkOrientationSerialization(2 * M_PI);
  checkOrientationSerialization(-70 / 180. * M_PI);
  checkOrientationSerialization(44 / 180. * M_PI);
  checkOrientationSerialization(58.9828 / 180. * M_PI);
}


//-----------------------------------------------------------------------------
TEST(TestSerialization, testPositionCoviaranceSerialization)
{
  checkPositionCovarianceSerialization(
    (Eigen::Matrix2d() << 1.189, 0.039, 0.039, 0.02).finished());

  checkPositionCovarianceSerialization(
    (Eigen::Matrix2d() << 1.0, 1.34, 0.039, 0.9).finished());

  EXPECT_ANY_THROW(
    checkPositionCovarianceSerialization(
      (Eigen::Matrix2d() << 9.0, -0.06, 0.01, 0.3).finished()));
}

//-----------------------------------------------------------------------------
TEST(TestSerialization, testOrientationViaranceSerialization)
{
  checkOrientationVarianceSerialization(0.1);
  EXPECT_ANY_THROW(checkOrientationVarianceSerialization(0.2));
}

//-----------------------------------------------------------------------------
TEST(TestSerialization, testPose2DSerialization)
{
  romea::core::Pose2D pose;
  pose.position.x() = 103.04892;
  pose.position.y() = -35.83893;
  pose.yaw = 190.098 * M_PI / 180.;
  pose.covariance.row(0) << 0.234, 0.00273, 0.0064765;
  pose.covariance.row(1) << 0.00273, 0.1435, -0.02593;
  pose.covariance.row(2) << 0.0064765, 0.02593, 0.033;

  auto buffer = romea::core::serializePose2D(pose);
  pose = romea::core::deserializePose2D(buffer);

  EXPECT_NEAR(pose.position.x(), 103.04892, 0.001);
  EXPECT_NEAR(pose.position.y(), -35.83893, 0.001);
  EXPECT_NEAR(pose.yaw, 190.098 * M_PI / 180., 0.01 * M_PI / 180.);
  EXPECT_NEAR(std::sqrt(pose.covariance(0, 0)), std::sqrt(0.234), 0.01);
  EXPECT_NEAR(std::sqrt(pose.covariance(1, 1)), std::sqrt(0.234), 0.01);
  EXPECT_NEAR(std::sqrt(pose.covariance(2, 2)), std::sqrt(0.033), 0.1 * M_PI / 180.);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
