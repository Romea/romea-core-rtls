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

#ifndef ROMEA_CORE_RTLS__SERIALIZATION__TWIST2D_HPP_
#define ROMEA_CORE_RTLS__SERIALIZATION__TWIST2D_HPP_

// std
#include <vector>

// romea
#include "romea_core_common/geometry/Twist2D.hpp"

namespace romea
{
namespace core
{

void serialize_linear_speed(const double & linear_speed, unsigned char * buffer);

void deserialize_linear_speed(const unsigned char * buffer, double & linear_speed);

void serialize_linear_speed_variance(const double & linear_speed_variance, unsigned char * buffer);

void deserialize_linear_speed_variance(
  const unsigned char * buffer, double & linear_speed_variance);

void serialize_angular_speed(const double & angular_speed, unsigned char * buffer);

void deserialize_angular_speed(const unsigned char * buffer, double & angular_speed);

void serialize_angular_speed_variance(
  const double & angular_speed_variance, unsigned char * buffer);

void deserialize_angular_speed_variance(
  const unsigned char * buffer, double & angular_speed_variance);

void serialize_linear_speeds(
  const Eigen::Ref<const Eigen::Vector2d> & linear_speeds, unsigned char * buffer);

void deserialize_linear_speeds(
  const unsigned char * buffer, Eigen::Ref<Eigen::Vector2d> linear_speeds);

void serialize_linear_speeds_covariance(
  const Eigen::Ref<const Eigen::Matrix2d> & linear_speeds_covariance, unsigned char * buffer);

void deserialize_linear_speeds_covariance(
  const unsigned char * buffer, Eigen::Ref<Eigen::Matrix2d> linear_speeds_covariance);

std::vector<unsigned char> serialize_twist2D(const Twist2D & twist);

Twist2D deserialize_twist2D(const std::vector<unsigned char> & twist);

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_RTLS__SERIALIZATION__TWIST2D_HPP_
