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

void check_linear_speed_serialization(const double& value) {
  double deserializedValue;
  std::vector<unsigned char> buffer(2);
  romea::core::serialize_linear_speed(value, buffer.data());
  romea::core::deserialize_linear_speed(buffer.data(), deserializedValue);
  EXPECT_NEAR(value, deserializedValue, 0.001);
}

void check_angular_speed_serialization(const double& value) {
  double deserializedValue;
  std::vector<unsigned char> buffer(2);
  romea::core::serialize_angular_speed(value, buffer.data());
  romea::core::deserialize_angular_speed(buffer.data(), deserializedValue);
  EXPECT_NEAR(deserializedValue, value, 0.01 * M_PI / 180.);
}

void check_linear_speeds_covariance_serialization(
    const Eigen::Matrix2d& covariance) {
  std::vector<unsigned char> buffer(1);
  Eigen::Matrix2d deserialize_linear_speeds_covariance;
  romea::core::serialize_linear_speeds_covariance(covariance, buffer.data());
  romea::core::deserialize_linear_speeds_covariance(
      buffer.data(), deserialize_linear_speeds_covariance);
  EXPECT_NEAR(std::sqrt(deserialize_linear_speeds_covariance(0, 0)),
              sqrt(covariance(0, 0)), 0.01);
  EXPECT_NEAR(std::sqrt(deserialize_linear_speeds_covariance(1, 1)),
              sqrt(covariance(1, 1)), 0.01);
  EXPECT_DOUBLE_EQ(deserialize_linear_speeds_covariance(0, 1), 0);
  EXPECT_DOUBLE_EQ(deserialize_linear_speeds_covariance(1, 0), 0);
}

void check_angular_speed_sariance_seerialization(const double& value) {
  double deserializedValue;
  std::vector<unsigned char> buffer(1);
  romea::core::serialize_angular_speed_variance(value, buffer.data());
  romea::core::deserialize_angular_speed_variance(buffer.data(),
                                                  deserializedValue);
  EXPECT_NEAR(deserializedValue, value, 0.1 * M_PI / 180.);
}

//-----------------------------------------------------------------------------
TEST(TestSerialization, testCartesianCoordinateSerialization) {
  check_linear_speed_serialization(27.778);
  check_linear_speed_serialization(10.012345);
  check_linear_speed_serialization(-9.87654);
  check_linear_speed_serialization(-27.778);
  EXPECT_ANY_THROW(check_linear_speed_serialization(30));
}

//-----------------------------------------------------------------------------
TEST(TestSerialization, testLinearSpeedsSerialization) {
  std::vector<unsigned char> buffer(8);
  Eigen::Vector2d deserialize_linear_speeds;
  romea::core::serialize_linear_speeds(Eigen::Vector2d(-10.2937, 3.2048),
                                       buffer.data());
  romea::core::deserialize_linear_speeds(buffer.data(),
                                         deserialize_linear_speeds);
  EXPECT_NEAR(deserialize_linear_speeds.x(), -10.2937, 0.001);
  EXPECT_NEAR(deserialize_linear_speeds.y(), 3.2048, 0.001);
}

//-----------------------------------------------------------------------------
TEST(TestSerialization, testOrientationSerialization) {
  check_angular_speed_serialization(-M_PI);
  check_angular_speed_serialization(M_PI);
  check_angular_speed_serialization(-0.3475);
  check_angular_speed_serialization(1.3895);
  EXPECT_ANY_THROW(check_angular_speed_serialization(M_PI + 0.1));
}

//-----------------------------------------------------------------------------
TEST(TestSerialization, testPositionCoviaranceSerialization) {
  check_linear_speeds_covariance_serialization(
      (Eigen::Matrix2d() << 1.189, 0.039, 0.039, 0.02).finished());

  check_linear_speeds_covariance_serialization(
      (Eigen::Matrix2d() << 1.0, 1.34, 0.039, 0.9).finished());

  EXPECT_ANY_THROW(check_linear_speeds_covariance_serialization(
      (Eigen::Matrix2d() << 9.0, -0.06, 0.01, 0.3).finished()));
}

//-----------------------------------------------------------------------------
TEST(TestSerialization, testAngularSpeedViaranceSerialization) {
  check_angular_speed_sariance_seerialization(0.1);
  check_angular_speed_sariance_seerialization(0.033);

  EXPECT_ANY_THROW(check_angular_speed_sariance_seerialization(0.2));
}

//-----------------------------------------------------------------------------
TEST(TestSerialization, testTwist2DSerialization) {
  romea::core::Twist2D twist;
  twist.linearSpeeds.x() = 10.304892;
  twist.linearSpeeds.y() = -3.583893;
  twist.angularSpeed = 20.098 * M_PI / 180.;
  twist.covariance.row(0) << 0.234, 0.00273, 0.0064765;
  twist.covariance.row(1) << 0.00273, 0.1435, -0.02593;
  twist.covariance.row(2) << 0.0064765, 0.02593, 0.033;

  auto buffer = romea::core::serialize_twist2D(twist);
  twist = romea::core::deserialize_twist2D(buffer);

  EXPECT_NEAR(twist.linearSpeeds.x(), 10.304892, 0.001);
  EXPECT_NEAR(twist.linearSpeeds.y(), -3.583893, 0.001);
  EXPECT_NEAR(twist.angularSpeed, 20.098 * M_PI / 180., 0.01 * M_PI / 180.);
  EXPECT_NEAR(std::sqrt(twist.covariance(0, 0)), std::sqrt(0.234), 0.01);
  EXPECT_NEAR(std::sqrt(twist.covariance(1, 1)), std::sqrt(0.1435), 0.01);
  EXPECT_NEAR(std::sqrt(twist.covariance(2, 2)), std::sqrt(0.033),
              0.1 * M_PI / 180.);
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
