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
#include <algorithm>
#include <cmath>
#include <vector>

// gtest
#include "gtest/gtest.h"

// romea
#include "romea_core_common/math/EulerAngles.hpp"
#include "romea_core_rtls/serialization/pose2D.hpp"

namespace
{

constexpr double kCartesianResolution = 0.001;
constexpr double kOrientationResolution = 0.01 * M_PI / 180.;
constexpr double kPositionStdResolution = 0.01;
constexpr double kOrientationVarianceStdResolution = 0.1 * M_PI / 180.;

void expect_cartesian_coordinate_round_trip(const double & expected_value)
{
  std::vector<unsigned char> buffer(4);
  double actual_value;

  romea::core::serialize_cartesian_coordinate(expected_value, buffer.data());
  romea::core::deserialize_cartesian_coordinate(buffer.data(), actual_value);

  EXPECT_NEAR(actual_value, expected_value, kCartesianResolution);
}

void expect_orientation_round_trip(const double & expected_yaw)
{
  std::vector<unsigned char> buffer(2);
  double actual_yaw;

  romea::core::serialize_orientation(expected_yaw, buffer.data());
  romea::core::deserialize_orientation(buffer.data(), actual_yaw);

  EXPECT_LE(
    std::abs(romea::core::betweenMinusPiAndPi(actual_yaw - expected_yaw)), kOrientationResolution);
}

void expect_position_covariance_round_trip(const Eigen::Matrix2d & expected_covariance)
{
  std::vector<unsigned char> buffer(1);
  Eigen::Matrix2d actual_covariance;

  romea::core::serialize_position2d_covariance(expected_covariance, buffer.data());
  romea::core::deserialize_position2d_covariance(buffer.data(), actual_covariance);

  const double expected_max_std =
    std::max(std::sqrt(expected_covariance(0, 0)), std::sqrt(expected_covariance(1, 1)));
  EXPECT_NEAR(std::sqrt(actual_covariance(0, 0)), expected_max_std, kPositionStdResolution);
  EXPECT_NEAR(std::sqrt(actual_covariance(1, 1)), expected_max_std, kPositionStdResolution);
  EXPECT_DOUBLE_EQ(actual_covariance(0, 1), 0);
  EXPECT_DOUBLE_EQ(actual_covariance(1, 0), 0);
}

void expect_orientation_variance_round_trip(const double & expected_variance)
{
  std::vector<unsigned char> buffer(1);
  double actual_variance;

  romea::core::serialize_orientation_variance(expected_variance, buffer.data());
  romea::core::deserialize_orientation_variance(buffer.data(), actual_variance);

  EXPECT_LE(
    std::abs(romea::core::betweenMinusPiAndPi(actual_variance - expected_variance)),
    kOrientationVarianceStdResolution);
}

}  // namespace

//-----------------------------------------------------------------------------
TEST(TestPose2DSerialization, cartesianCoordinateRoundTrip)
{
  expect_cartesian_coordinate_round_trip(1000000);
  expect_cartesian_coordinate_round_trip(10.012345);
  expect_cartesian_coordinate_round_trip(-987654.012345);
  expect_cartesian_coordinate_round_trip(-1000000);
}

//-----------------------------------------------------------------------------
TEST(TestPose2DSerialization, cartesianCoordinateRejectsOutOfRangeValues)
{
  EXPECT_ANY_THROW(expect_cartesian_coordinate_round_trip(1000000.1));
}

//-----------------------------------------------------------------------------
TEST(TestPose2DSerialization, position2DRoundTrip)
{
  std::vector<unsigned char> buffer(8);
  Eigen::Vector2d actual_position;

  romea::core::serialize_position2d(Eigen::Vector2d(-100.2937, 320.4873), buffer.data());
  romea::core::deserialize_position2d(buffer.data(), actual_position);

  EXPECT_NEAR(actual_position.x(), -100.2937, kCartesianResolution);
  EXPECT_NEAR(actual_position.y(), 320.4873, kCartesianResolution);
}

//-----------------------------------------------------------------------------
TEST(TestPose2DSerialization, orientationRoundTrip)
{
  expect_orientation_round_trip(-M_PI);
  expect_orientation_round_trip(2 * M_PI);
  expect_orientation_round_trip(-70 / 180. * M_PI);
  expect_orientation_round_trip(44 / 180. * M_PI);
  expect_orientation_round_trip(58.9828 / 180. * M_PI);
}

//-----------------------------------------------------------------------------
TEST(TestPose2DSerialization, positionCovarianceRoundTrip)
{
  expect_position_covariance_round_trip(
    (Eigen::Matrix2d() << 1.189, 0.039, 0.039, 0.02).finished());

  expect_position_covariance_round_trip((Eigen::Matrix2d() << 1.0, 0.039, 0.039, 0.9).finished());
}

//-----------------------------------------------------------------------------
TEST(TestPose2DSerialization, positionCovarianceRejectsInvalidValues)
{
  EXPECT_ANY_THROW(
    expect_position_covariance_round_trip((Eigen::Matrix2d() << -0.1, 0.0, 0.0, 0.3).finished()));

  EXPECT_ANY_THROW(
    expect_position_covariance_round_trip((Eigen::Matrix2d() << 9.0, -0.06, 0.01, 0.3).finished()));
}

//-----------------------------------------------------------------------------
TEST(TestPose2DSerialization, orientationVarianceRoundTrip)
{
  expect_orientation_variance_round_trip(0.1);
}

//-----------------------------------------------------------------------------
TEST(TestPose2DSerialization, orientationVarianceRejectsInvalidValues)
{
  EXPECT_ANY_THROW(expect_orientation_variance_round_trip(-0.1));
  EXPECT_ANY_THROW(expect_orientation_variance_round_trip(0.2));
}

//-----------------------------------------------------------------------------
TEST(TestPose2DSerialization, pose2DRoundTrip)
{
  romea::core::Pose2D expected_pose;
  expected_pose.position.x() = 103.04892;
  expected_pose.position.y() = -35.83893;
  expected_pose.yaw = 190.098 * M_PI / 180.;
  expected_pose.covariance.row(0) << 0.234, 0.00273, 0.0064765;
  expected_pose.covariance.row(1) << 0.00273, 0.1435, -0.02593;
  expected_pose.covariance.row(2) << 0.0064765, 0.02593, 0.033;

  auto buffer = romea::core::serialize_pose2d(expected_pose);
  auto actual_pose = romea::core::deserialize_pose2d(buffer);

  EXPECT_NEAR(actual_pose.position.x(), expected_pose.position.x(), kCartesianResolution);
  EXPECT_NEAR(actual_pose.position.y(), expected_pose.position.y(), kCartesianResolution);
  EXPECT_NEAR(actual_pose.yaw, expected_pose.yaw, kOrientationResolution);
  EXPECT_NEAR(std::sqrt(actual_pose.covariance(0, 0)), std::sqrt(0.234), kPositionStdResolution);
  EXPECT_NEAR(std::sqrt(actual_pose.covariance(1, 1)), std::sqrt(0.234), kPositionStdResolution);
  EXPECT_NEAR(
    std::sqrt(actual_pose.covariance(2, 2)),
    std::sqrt(expected_pose.covariance(2, 2)),
    kOrientationVarianceStdResolution);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
