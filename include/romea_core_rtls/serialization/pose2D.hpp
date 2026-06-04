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

#ifndef ROMEA_CORE_RTLS__SERIALIZATION__POSE2D_HPP_
#define ROMEA_CORE_RTLS__SERIALIZATION__POSE2D_HPP_

// eigen
#include <Eigen/Core>

// std
#include <vector>

// romea
#include "romea_core_common/geometry/Pose2D.hpp"

namespace romea
{
namespace core
{

void serialize_cartesian_coordinate(const double & cartesian_coordinate, unsigned char * buffer);

void deserialize_cartesian_coordinate(const unsigned char * buffer, double & cartesian_coordinate);

void serialize_wgs84_coordinate(double & wgs84_coordinate, unsigned char * buffer);

void deserialize_wgs84_coordinate(const unsigned char * buffer, double & wgs84_coordinate);

void serialize_position2d(
  const Eigen::Ref<const Eigen::Vector2d> & position, unsigned char * buffer);

void deserialize_position2d(const unsigned char * buffer, Eigen::Ref<Eigen::Vector2d> position);

void deserialize_position2d_covariance(
  const unsigned char * buffer, Eigen::Ref<Eigen::Matrix2d> position_covariance);

void serialize_position2d_covariance(
  const Eigen::Ref<const Eigen::Matrix2d> & position_covariance, unsigned char * buffer);

void serialize_orientation(const double & orientation, unsigned char * buffer);

void deserialize_orientation(const unsigned char * buffer, double & orientation);

void serialize_orientation_variance(const double & orientation_covariance, unsigned char * buffer);

void deserialize_orientation_variance(
  const unsigned char * buffer, double & orientation_covariance);

std::vector<unsigned char> serialize_pose2d(const Pose2D & pose);

Pose2D deserialize_pose2d(const std::vector<unsigned char> & buffer);

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_RTLS__SERIALIZATION__POSE2D_HPP_
