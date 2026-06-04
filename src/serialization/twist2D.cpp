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
#include <exception>
#include <vector>

// romea
#include "romea_core_common/math/Matrix.hpp"
#include "romea_core_rtls/serialization/twist2D.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
void serialize_linear_speed(const double & value, unsigned char * buffer)
{
  if (std::abs(value) > 27.778) {
    throw std::runtime_error(
      "Cannot serialize linear speed because its absolute value is greater "
      "than 100km/h");
  }

  *reinterpret_cast<uint16_t *>(buffer) = static_cast<uint16_t>(value * 1000) + 32768;
}

//-----------------------------------------------------------------------------
void deserialize_linear_speed(const unsigned char * buffer, double & value)
{
  value = (*reinterpret_cast<const uint16_t *>(buffer) - 32768) / 1000.0;
}

//-----------------------------------------------------------------------------
void serialize_linear_speed_variance(const double & value, unsigned char * buffer)
{
  if (value < 0.0 || value > 4.0) {
    throw std::runtime_error(
      "Cannot serialize linear speed variance because its value is outside "
      "[0, 4]");
  }

  double std = std::sqrt(value);
  *buffer = std::ceil(std * 100);
}

//-----------------------------------------------------------------------------
void deserialize_linear_speed_variance(const unsigned char * buffer, double & value)
{
  value = std::pow(*buffer / 100.0, 2);
}

//-----------------------------------------------------------------------------
void serialize_angular_speed(const double & value, unsigned char * buffer)
{
  if (std::abs(value) > M_PI) {
    throw std::runtime_error(
      "Cannot serialize angular speed because its absolute value is greater "
      "180deg/s");
  }

  *reinterpret_cast<uint16_t *>(buffer) = static_cast<uint16_t>(value / M_PI * 18000) + 32768;
}

//-----------------------------------------------------------------------------
void deserialize_angular_speed(const unsigned char * buffer, double & value)
{
  value = (*reinterpret_cast<const uint16_t *>(buffer) - 32768) * M_PI / 18000.0;
}

//-----------------------------------------------------------------------------
void serialize_angular_speed_variance(const double & value, unsigned char * buffer)
{
  if (value < 0.0 || value > 0.199) {
    throw std::runtime_error(
      "Cannot serialize angular speed variance because its value is outside "
      "[0, 0.2]");
  }
  *buffer = std::ceil(std::sqrt(value) / M_PI * 1800);
}

//-----------------------------------------------------------------------------
void deserialize_angular_speed_variance(const unsigned char * buffer, double & value)
{
  value = std::pow((*buffer * M_PI) / 1800., 2);
}

//-----------------------------------------------------------------------------
void serialize_linear_speeds(
  const Eigen::Ref<const Eigen::Vector2d> & linearSpeeds, unsigned char * buffer)
{
  serialize_linear_speed(linearSpeeds.x(), buffer);
  serialize_linear_speed(linearSpeeds.y(), buffer + 2);
}

//-----------------------------------------------------------------------------
void deserialize_linear_speeds(
  const unsigned char * buffer, Eigen::Ref<Eigen::Vector2d> linearSpeeds)
{
  deserialize_linear_speed(buffer, linearSpeeds.x());
  deserialize_linear_speed(buffer + 2, linearSpeeds.y());
}

//-----------------------------------------------------------------------------
void serialize_linear_speeds_covariance(
  const Eigen::Ref<const Eigen::Matrix2d> & covariance, unsigned char * buffer)
{
  if (!isPositiveSemiDefiniteMatrix(covariance)) {
    throw std::runtime_error(
      "Cannot serialize linear speeds covariance because it is not positive "
      "semi-definite");
  }

  serialize_linear_speed_variance(covariance(0, 0), buffer);
  serialize_linear_speed_variance(covariance(1, 1), buffer + 1);
}

//-----------------------------------------------------------------------------
void deserialize_linear_speeds_covariance(
  const unsigned char * buffer, Eigen::Ref<Eigen::Matrix2d> covariance)
{
  covariance(0, 1) = 0;
  covariance(1, 0) = 0;
  deserialize_linear_speed_variance(buffer, covariance(0, 0));
  deserialize_linear_speed_variance(buffer + 1, covariance(1, 1));
}

//-----------------------------------------------------------------------------
std::vector<unsigned char> serialize_twist2D(const Twist2D & twist)
{
  std::vector<unsigned char> buffer(9);
  serialize_linear_speeds(twist.linearSpeeds, buffer.data());
  serialize_angular_speed(twist.angularSpeed, buffer.data() + 4);
  serialize_linear_speeds_covariance(twist.covariance.block<2, 2>(0, 0), buffer.data() + 6);
  serialize_angular_speed_variance(twist.covariance(2, 2), buffer.data() + 8);
  return buffer;
}

//-----------------------------------------------------------------------------
Twist2D deserialize_twist2D(const std::vector<unsigned char> & buffer)
{
  Twist2D twist;
  deserialize_linear_speeds(buffer.data(), twist.linearSpeeds);
  deserialize_angular_speed(buffer.data() + 4, twist.angularSpeed);
  deserialize_linear_speeds_covariance(buffer.data() + 6, twist.covariance.block<2, 2>(0, 0));
  deserialize_angular_speed_variance(buffer.data() + 8, twist.covariance(2, 2));
  return twist;
}

}  // namespace core
}  // namespace romea
