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
#include <exception>

// romea
#include "romea_core_rtls/serialization/Twist2DSerialization.hpp"

namespace romea
{

//-----------------------------------------------------------------------------
void serializeLinearSpeed(const double & value, unsigned char * buffer)
{
  if (value > 27.778) {
    throw std::runtime_error(
            "Cannot serialize linear speed because it's value is greater than 100km/h");
  }

  *reinterpret_cast<uint16_t *>(buffer) = static_cast<uint16_t>(value * 1000) + 32768;
}

//-----------------------------------------------------------------------------
void deserializeLinearSpeed(const unsigned char * buffer, double & value)
{
  value = (*reinterpret_cast<const uint16_t *>(buffer) - 32768) / 1000.0;
}

//-----------------------------------------------------------------------------
void serializeLinearSpeedVariance(const double & value, unsigned char * buffer)
{
  double std = std::sqrt(value);

  if (std::abs(value) > 2.) {
    throw std::runtime_error(
            "Cannot serialize linear speed variance because it's value is greater than 4");
  }

  *buffer = std::ceil(std * 100);
}

//-----------------------------------------------------------------------------
void deserializeLinearSpeedVariance(const unsigned char * buffer, double & value)
{
  value = std::pow(*buffer / 100.0, 2);
}

//-----------------------------------------------------------------------------
void serializeAngularSpeed(const double & value, unsigned char * buffer)
{
  if (std::abs(value) > M_PI) {
    throw std::runtime_error(
            "Cannot serialize linear speed because it's value is greater than 180deg/s");
  }

  *reinterpret_cast<uint16_t *>(buffer) = static_cast<uint16_t>(value / M_PI * 18000) + 32768;
}

//-----------------------------------------------------------------------------
void deserializeAngularSpeed(const unsigned char * buffer, double & value)
{
  value = (*reinterpret_cast<const uint16_t *>(buffer) - 32768) * M_PI / 18000.0;
}

//-----------------------------------------------------------------------------
void serializeAngularSpeedVariance(const double & value, unsigned char * buffer)
{
  if (value > 0.199) {
    throw std::runtime_error(
            "Cannot serialize orientation variance because it's value is greater than 0.2");
  }
  *buffer = std::ceil(std::sqrt(value) / M_PI * 1800);
}

//-----------------------------------------------------------------------------
void deserializeAngularSpeedVariance(const unsigned char * buffer, double & value)
{
  value = std::pow((*buffer * M_PI) / 1800., 2);
}

//-----------------------------------------------------------------------------
void serializeLinearSpeeds(
  const Eigen::Ref<const Eigen::Vector2d> & linearSpeeds,
  unsigned char * buffer)
{
  serializeLinearSpeed(linearSpeeds.x(), buffer);
  serializeLinearSpeed(linearSpeeds.y(), buffer + 2);
}

//-----------------------------------------------------------------------------
void deserializeLinearSpeeds(
  const unsigned char * buffer,
  Eigen::Ref<Eigen::Vector2d> linearSpeeds)
{
  deserializeLinearSpeed(buffer, linearSpeeds.x());
  deserializeLinearSpeed(buffer + 2, linearSpeeds.y());
}

//-----------------------------------------------------------------------------
void serializeLinearSpeedsCovariance(
  const Eigen::Ref<const Eigen::Matrix2d> & covariance,
  unsigned char * buffer)
{
  serializeLinearSpeedVariance(covariance(0, 0), buffer);
  serializeLinearSpeedVariance(covariance(1, 1), buffer + 1);
}

//-----------------------------------------------------------------------------
void deserializeLinearSpeedsCovariance(
  const unsigned char * buffer,
  Eigen::Ref<Eigen::Matrix2d> covariance)
{
  covariance(0, 1) = 0;
  covariance(1, 0) = 0;
  deserializeLinearSpeedVariance(buffer, covariance(0, 0));
  deserializeLinearSpeedVariance(buffer + 1, covariance(1, 1));
}

//-----------------------------------------------------------------------------
std::vector<unsigned char> serializeTwist2D(const Twist2D & twist)
{
  std::vector<unsigned char> buffer(9);
  serializeLinearSpeeds(twist.linearSpeeds, buffer.data());
  serializeAngularSpeed(twist.angularSpeed, buffer.data() + 4);
  serializeLinearSpeedsCovariance(twist.covariance.block<2, 2>(0, 0), buffer.data() + 6);
  serializeAngularSpeedVariance(twist.covariance(2, 2), buffer.data() + 8);
  return buffer;
}

//-----------------------------------------------------------------------------
Twist2D deserializeTwist2D(const std::vector<unsigned char> & buffer)
{
  Twist2D twist;
  deserializeLinearSpeeds(buffer.data(), twist.linearSpeeds);
  deserializeAngularSpeed(buffer.data() + 4, twist.angularSpeed);
  deserializeLinearSpeedsCovariance(buffer.data() + 6, twist.covariance.block<2, 2>(0, 0));
  deserializeAngularSpeedVariance(buffer.data() + 8, twist.covariance(2, 2));
  return twist;
}

}  // namespace romea
