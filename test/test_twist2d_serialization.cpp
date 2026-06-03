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
#include <cmath>
#include <vector>

// gtest
#include "gtest/gtest.h"

// romea
#include "romea_core_rtls/serialization/twist2D.hpp"

namespace {

constexpr double kLinearSpeedResolution = 0.001;
constexpr double kAngularSpeedResolution = 0.01 * M_PI / 180.;
constexpr double kVarianceStdResolution = 0.01;
constexpr double kAngularVarianceStdResolution = 0.1 * M_PI / 180.;

void expect_linear_speed_round_trip(const double& expected_speed) {
  std::vector<unsigned char> buffer(2);
  double actual_speed;

  romea::core::serialize_linear_speed(expected_speed, buffer.data());
  romea::core::deserialize_linear_speed(buffer.data(), actual_speed);

  EXPECT_NEAR(actual_speed, expected_speed, kLinearSpeedResolution);
}

void expect_angular_speed_round_trip(const double& expected_speed) {
  std::vector<unsigned char> buffer(2);
  double actual_speed;

  romea::core::serialize_angular_speed(expected_speed, buffer.data());
  romea::core::deserialize_angular_speed(buffer.data(), actual_speed);

  EXPECT_NEAR(actual_speed, expected_speed, kAngularSpeedResolution);
}

void expect_linear_speeds_covariance_round_trip(
    const Eigen::Matrix2d& expected_covariance) {
  std::vector<unsigned char> buffer(2);
  Eigen::Matrix2d actual_covariance;

  romea::core::serialize_linear_speeds_covariance(expected_covariance,
                                                  buffer.data());
  romea::core::deserialize_linear_speeds_covariance(buffer.data(),
                                                    actual_covariance);

  EXPECT_NEAR(std::sqrt(actual_covariance(0, 0)),
              std::sqrt(expected_covariance(0, 0)), kVarianceStdResolution);
  EXPECT_NEAR(std::sqrt(actual_covariance(1, 1)),
              std::sqrt(expected_covariance(1, 1)), kVarianceStdResolution);
  EXPECT_DOUBLE_EQ(actual_covariance(0, 1), 0);
  EXPECT_DOUBLE_EQ(actual_covariance(1, 0), 0);
}

void expect_angular_speed_variance_round_trip(
    const double& expected_variance) {
  std::vector<unsigned char> buffer(1);
  double actual_variance;

  romea::core::serialize_angular_speed_variance(expected_variance,
                                                buffer.data());
  romea::core::deserialize_angular_speed_variance(buffer.data(),
                                                  actual_variance);

  EXPECT_NEAR(actual_variance, expected_variance,
              kAngularVarianceStdResolution);
}

}  // namespace

//-----------------------------------------------------------------------------
TEST(TestTwist2DSerialization, linearSpeedRoundTrip) {
  expect_linear_speed_round_trip(27.778);
  expect_linear_speed_round_trip(10.012345);
  expect_linear_speed_round_trip(-9.87654);
  expect_linear_speed_round_trip(-27.778);
}

//-----------------------------------------------------------------------------
TEST(TestTwist2DSerialization, linearSpeedRejectsOutOfRangeValues) {
  EXPECT_ANY_THROW(expect_linear_speed_round_trip(30));
  EXPECT_ANY_THROW(expect_linear_speed_round_trip(-30));
}

//-----------------------------------------------------------------------------
TEST(TestTwist2DSerialization, linearSpeedsRoundTrip) {
  std::vector<unsigned char> buffer(4);
  Eigen::Vector2d actual_linear_speeds;

  romea::core::serialize_linear_speeds(Eigen::Vector2d(-10.2937, 3.2048),
                                       buffer.data());
  romea::core::deserialize_linear_speeds(buffer.data(),
                                         actual_linear_speeds);

  EXPECT_NEAR(actual_linear_speeds.x(), -10.2937, kLinearSpeedResolution);
  EXPECT_NEAR(actual_linear_speeds.y(), 3.2048, kLinearSpeedResolution);
}

//-----------------------------------------------------------------------------
TEST(TestTwist2DSerialization, angularSpeedRoundTrip) {
  expect_angular_speed_round_trip(-M_PI);
  expect_angular_speed_round_trip(M_PI);
  expect_angular_speed_round_trip(-0.3475);
  expect_angular_speed_round_trip(1.3895);
}

//-----------------------------------------------------------------------------
TEST(TestTwist2DSerialization, angularSpeedRejectsOutOfRangeValues) {
  EXPECT_ANY_THROW(expect_angular_speed_round_trip(M_PI + 0.1));
}

//-----------------------------------------------------------------------------
TEST(TestTwist2DSerialization, linearSpeedsCovarianceRoundTrip) {
  expect_linear_speeds_covariance_round_trip(
      (Eigen::Matrix2d() << 1.189, 0.039, 0.039, 0.02).finished());

  expect_linear_speeds_covariance_round_trip(
      (Eigen::Matrix2d() << 1.0, 0.039, 0.039, 0.9).finished());
}

//-----------------------------------------------------------------------------
TEST(TestTwist2DSerialization, linearSpeedsCovarianceRejectsInvalidValues) {
  EXPECT_ANY_THROW(expect_linear_speeds_covariance_round_trip(
      (Eigen::Matrix2d() << -0.1, 0.0, 0.0, 0.3).finished()));

  EXPECT_ANY_THROW(expect_linear_speeds_covariance_round_trip(
      (Eigen::Matrix2d() << 9.0, -0.06, 0.01, 0.3).finished()));
}

//-----------------------------------------------------------------------------
TEST(TestTwist2DSerialization, angularSpeedVarianceRoundTrip) {
  expect_angular_speed_variance_round_trip(0.1);
  expect_angular_speed_variance_round_trip(0.033);
}

//-----------------------------------------------------------------------------
TEST(TestTwist2DSerialization, angularSpeedVarianceRejectsInvalidValues) {
  EXPECT_ANY_THROW(expect_angular_speed_variance_round_trip(-0.1));
  EXPECT_ANY_THROW(expect_angular_speed_variance_round_trip(0.2));
}

//-----------------------------------------------------------------------------
TEST(TestTwist2DSerialization, twist2DRoundTrip) {
  romea::core::Twist2D expected_twist;
  expected_twist.linearSpeeds.x() = 10.304892;
  expected_twist.linearSpeeds.y() = -3.583893;
  expected_twist.angularSpeed = 20.098 * M_PI / 180.;
  expected_twist.covariance.row(0) << 0.234, 0.00273, 0.0064765;
  expected_twist.covariance.row(1) << 0.00273, 0.1435, -0.02593;
  expected_twist.covariance.row(2) << 0.0064765, 0.02593, 0.033;

  auto buffer = romea::core::serialize_twist2D(expected_twist);
  auto actual_twist = romea::core::deserialize_twist2D(buffer);

  EXPECT_NEAR(actual_twist.linearSpeeds.x(), expected_twist.linearSpeeds.x(),
              kLinearSpeedResolution);
  EXPECT_NEAR(actual_twist.linearSpeeds.y(), expected_twist.linearSpeeds.y(),
              kLinearSpeedResolution);
  EXPECT_NEAR(actual_twist.angularSpeed, expected_twist.angularSpeed,
              kAngularSpeedResolution);
  EXPECT_NEAR(std::sqrt(actual_twist.covariance(0, 0)),
              std::sqrt(expected_twist.covariance(0, 0)),
              kVarianceStdResolution);
  EXPECT_NEAR(std::sqrt(actual_twist.covariance(1, 1)),
              std::sqrt(expected_twist.covariance(1, 1)),
              kVarianceStdResolution);
  EXPECT_NEAR(std::sqrt(actual_twist.covariance(2, 2)),
              std::sqrt(expected_twist.covariance(2, 2)),
              kAngularVarianceStdResolution);
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
