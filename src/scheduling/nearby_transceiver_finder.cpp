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
#include <algorithm>
#include <iostream>
#include <vector>

// romea
#include "romea_core_rtls/scheduling/nearby_transceiver_finder.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
RTLSNearbyTransceiverFinder::RTLSNearbyTransceiverFinder(
  const VectorOfEigenVector3d & points, const double & research_radius)
: squared_research_radius_(research_radius * research_radius),
  points_(points),
  kdtree_(points_),
  neighbor_indexes_(points.size()),
  neighbor_square_distances_(points.size())
{
}

//-----------------------------------------------------------------------------
const std::vector<size_t> & RTLSNearbyTransceiverFinder::find(const Eigen::Vector3d & position)
{
  kdtree_.radiusResearch(
    position, squared_research_radius_, neighbor_indexes_, neighbor_square_distances_);

  std::sort(neighbor_indexes_.begin(), neighbor_indexes_.end());
  return neighbor_indexes_;
}

}  // namespace core
}  // namespace romea
