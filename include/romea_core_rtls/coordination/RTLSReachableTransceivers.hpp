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


#ifndef ROMEA_CORE_RTLS__COORDINATION__RTLSREACHABLETRANSCEIVERS_HPP_
#define ROMEA_CORE_RTLS__COORDINATION__RTLSREACHABLETRANSCEIVERS_HPP_

// std
#include <vector>

// romea
#include "romea_core_common/pointset/KdTree.hpp"

namespace romea
{
namespace core
{

class RTLSReachableTransceivers
{
public:
  RTLSReachableTransceivers(
    const VectorOfEigenVector3d & points,
    const double & researchRadius);

  const std::vector<size_t> & find(const Eigen::Vector3d & position);

private:
  double squaredResearchRadius_;
  VectorOfEigenVector3d points_;
  KdTree<Eigen::Vector3d> kdTree_;
  std::vector<size_t> neighborIndexes_;
  std::vector<double> neighborSquareDistances_;
};

}   // namespace core
}   // namespace romea

#endif  // ROMEA_CORE_RTLS__COORDINATION__RTLSREACHABLETRANSCEIVERS_HPP_
