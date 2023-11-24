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

#include <iostream>
#include "romea_core_rtls/coordination/RTLSReachableTransceivers.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
RTLSReachableTransceivers::RTLSReachableTransceivers(
  const VectorOfEigenVector3d & points,
  const double & researchRadius)
: squaredResearchRadius_(researchRadius * researchRadius),
  points_(points),
  kdTree_(points_),
  neighborIndexes_(points.size()),
  neighborSquareDistances_(points.size())
{
}

//-----------------------------------------------------------------------------
const std::vector<size_t> & RTLSReachableTransceivers::find(const Eigen::Vector3d & position)
{
  kdTree_.radiusResearch(
    position, squaredResearchRadius_, neighborIndexes_, neighborSquareDistances_);

  std::sort(neighborIndexes_.begin(), neighborIndexes_.end());
  return neighborIndexes_;
}

}   // namespace core
}   // namespace romea
