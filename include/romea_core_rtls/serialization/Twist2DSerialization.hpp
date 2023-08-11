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


#ifndef ROMEA_CORE_RTLS__SERIALIZATION__TWIST2DSERIALIZATION_HPP_
#define ROMEA_CORE_RTLS__SERIALIZATION__TWIST2DSERIALIZATION_HPP_

// std
#include <vector>

// romea
#include "romea_core_common/geometry/Twist2D.hpp"


namespace romea
{

void serializeLinearSpeed(const double & value, unsigned char * buffer);

void deserializeLinearSpeed(const unsigned char * buffer, double & value);

void serializeLinearSpeedVariance(const double & value, unsigned char * buffer);

void deserializeLinearSpeedVariance(const unsigned char * buffer, double & value);

void serializeAngularSpeed(const double & value, unsigned char * buffer);

void deserializeAngularSpeed(const unsigned char * buffer, double & value);

void serializeAngularSpeedVariance(const double & value, unsigned char * buffer);

void deserializeAngularSpeedVariance(const unsigned char * buffer, double & value);

void serializeLinearSpeeds(
  const Eigen::Ref<const Eigen::Vector2d> & linearSpeeds,
  unsigned char * buffer);

void deserializeLinearSpeeds(
  const unsigned char * buffer,
  Eigen::Ref<Eigen::Vector2d> linearSpeeds);

void serializeLinearSpeedsCovariance(
  const Eigen::Ref<const Eigen::Matrix2d> & covariance,
  unsigned char * buffer);

void deserializeLinearSpeedsCovariance(
  const unsigned char * buffer,
  Eigen::Ref<Eigen::Matrix2d> covariance);


std::vector<unsigned char> serializeTwist2D(const Twist2D & twist);

Twist2D deserializeTwist2D(const std::vector<unsigned char> & twist);

}  // namespace romea

#endif  // ROMEA_CORE_RTLS__SERIALIZATION__TWIST2DSERIALIZATION_HPP_
