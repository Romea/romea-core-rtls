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


#ifndef ROMEA_CORE_RTLS__SERIALIZATION__POSE2DSERIALIZATION_HPP_
#define ROMEA_CORE_RTLS__SERIALIZATION__POSE2DSERIALIZATION_HPP_

// eigen
#include <Eigen/Core>

// std
#include <vector>

// romea
#include "romea_core_common/geometry/Pose2D.hpp"


namespace romea
{

void serializeCartesianCoordinate(const double & value, unsigned char * buffer);
void deserializeCartesianCoordinate(const unsigned char * buffer, double & value);

void serializeWGS84Coordinate(double & value, unsigned char * buffer);
void deserializeWGS84Coordinate(const unsigned char * buffer, double & value);

void serialiazeOrientation(const double & value, unsigned char * buffer);
void deserializeOrientation(const unsigned char * buffer, double & value);

void serializeOrientationVariance(const double & value, unsigned char * buffer);
void deserializeOrientationVariance(const unsigned char * buffer, double & value);


void serializeCartesianPosition(
  const Eigen::Ref<const Eigen::Vector2d> & position,
  unsigned char * buffer);

void deserializeCartesianPosition(
  const unsigned char * buffer,
  Eigen::Ref<Eigen::Vector2d> position);

void serializePositionCovariance(
  const Eigen::Ref<const Eigen::Matrix2d> & covariance,
  unsigned char * buffer);

void deserializePositionCovariance(
  const unsigned char * buffer,
  Eigen::Ref<Eigen::Matrix2d> covariance);


std::vector<unsigned char> serializePose2D(const Pose2D & pose);
Pose2D deserializePose2D(const std::vector<unsigned char> & buffer);


}  // namespace romea

#endif  // ROMEA_CORE_RTLS__SERIALIZATION__POSE2DSERIALIZATION_HPP_
