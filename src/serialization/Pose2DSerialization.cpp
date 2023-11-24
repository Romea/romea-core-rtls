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
#include <exception>

// romea
#include "romea_core_common/math/EulerAngles.hpp"
#include "romea_core_common/geometry/Pose2D.hpp"


namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
void serializeCartesianCoordinate(const double & value, unsigned char * buffer)
{
  if (std::abs(value) > 1000000) {
    throw std::runtime_error(
            "Cannot serialize cartesian coordinate because it's value is greater than 1000km");
  }

  *reinterpret_cast<uint32_t *>(buffer) = static_cast<uint32_t>(value * 1000) + 2147483648;
}

//-----------------------------------------------------------------------------
void deserializeCartesianCoordinate(const unsigned char * buffer, double & value)
{
  value = (*reinterpret_cast<const uint32_t *>(buffer) - 2147483648) / 1000.;
}

//-----------------------------------------------------------------------------
void serializeWGS84Coordinate(double & value, unsigned char * buffer)
{
  double angle = between0And2Pi(value);
  double angleInDegree = std::abs(180. * angle / M_PI);

  uint16_t degrees = angleInDegree;
  uint16_t minutes = (angleInDegree - degrees) * 60;
  uint16_t seconds = ((angleInDegree - degrees) * 60 - minutes) * 60000;
  *reinterpret_cast<uint16_t *>(buffer) = degrees * 60 + minutes;
  *reinterpret_cast<uint16_t *>(buffer + 2) = seconds;
}

//-----------------------------------------------------------------------------
void deserializeWGS84Coordinate(const unsigned char * buffer, double & value)
{
  uint16_t seconds = *reinterpret_cast<const uint16_t *>(buffer + 2);
  uint16_t minutes = *reinterpret_cast<const uint16_t *>(buffer);
  uint16_t degrees = minutes % 60;

  double angleInDegree = (degrees + (minutes - degrees * 60) / 60. + seconds / 3600000.);
  value = betweenMinusPiAndPi(angleInDegree / 180. * M_PI);
}

//-----------------------------------------------------------------------------
void serialiazeOrientation(const double & value, unsigned char * buffer)
{
  *reinterpret_cast<uint16_t *>(buffer) = (value / M_PI) * 18000 + 18000;
}

//-----------------------------------------------------------------------------
void deserializeOrientation(const unsigned char * buffer, double & value)
{
  value = (*reinterpret_cast<const uint16_t *>(buffer) - 18000) * M_PI / 18000.;
}

//-----------------------------------------------------------------------------
void serializeOrientationVariance(const double & value, unsigned char * buffer)
{
  if (value > 0.199) {
    throw std::runtime_error(
            "Cannot serialize orientation variance because it's value is greater than 0.2");
  }
  *buffer = std::ceil(std::sqrt(value) / M_PI * 1800);
}

//-----------------------------------------------------------------------------
void deserializeOrientationVariance(const unsigned char * buffer, double & value)
{
  value = std::pow((*buffer * M_PI) / 1800., 2);
}

//-----------------------------------------------------------------------------
void serializeCartesianPosition(
  const Eigen::Ref<const Eigen::Vector2d> & position,
  unsigned char * buffer)
{
  serializeCartesianCoordinate(position.x(), buffer);
  serializeCartesianCoordinate(position.y(), buffer + 4);
}

//-----------------------------------------------------------------------------
void deserializeCartesianPosition(
  const unsigned char * buffer,
  Eigen::Ref<Eigen::Vector2d> position)
{
  deserializeCartesianCoordinate(buffer, position.x());
  deserializeCartesianCoordinate(buffer + 4, position.y());
}

//-----------------------------------------------------------------------------
void serializePositionCovariance(
  const Eigen::Ref<const Eigen::Matrix2d> & covariance,
  unsigned char * buffer)
{
  double std = std::max(std::sqrt(covariance(0, 0)), std::sqrt(covariance(1, 1)));

  if (std::abs(std) > 2.) {
    throw std::runtime_error(
            "Cannot serialize position covariance because one of variance value is greater than 4");
  }

  *buffer = std::ceil(std * 100);
}

//-----------------------------------------------------------------------------
void deserializePositionCovariance(
  const unsigned char * buffer,
  Eigen::Ref<Eigen::Matrix2d> covariance)
{
  double std = *buffer / 100.;
  covariance = Eigen::Matrix2d::Identity() * std * std;
}


//-----------------------------------------------------------------------------
std::vector<unsigned char> serializePose2D(const Pose2D & pose)
{
  std::vector<unsigned char> buffer(12);
  serializeCartesianPosition(pose.position, buffer.data());
  serialiazeOrientation(pose.yaw, buffer.data() + 8);
  serializePositionCovariance(pose.covariance.block<2, 2>(0, 0), buffer.data() + 10);
  serializeOrientationVariance(pose.covariance(2, 2), buffer.data() + 11);
  return buffer;
}

//-----------------------------------------------------------------------------
Pose2D deserializePose2D(const std::vector<uint8_t> & buffer)
{
  Pose2D pose;
  deserializeCartesianPosition(buffer.data(), pose.position);
  deserializeOrientation(buffer.data() + 8, pose.yaw);
  deserializePositionCovariance(buffer.data() + 10, pose.covariance.block<2, 2>(0, 0));
  deserializeOrientationVariance(buffer.data() + 11, pose.covariance(2, 2));
  return pose;
}

}  // namespace core
}  // namespace romea
