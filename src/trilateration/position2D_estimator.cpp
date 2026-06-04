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

#include "romea_core_rtls/trilateration/position2D_estimator.hpp"

#include "romea_core_rtls/trilateration/simple_trilateration2D.hpp"

namespace
{
const size_t MINIMAL_NUMBER_OF_RANGES_TO_COMPUTE_POSITION = 2;
}

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
RTLSPosition2DEstimator::RTLSPosition2DEstimator(
  const VectorOfEigenVector3d & referencetag_positions, const double & estimateEpsilon)
: NLSE(estimateEpsilon)
{
  estimate_.resize(2);
  estimateCovariance_.resize(2, 2);
  leastSquares_.setEstimateSize(2);

  ranges_.resize(referencetag_positions.size());
  reference_tag_positions_.resize(referencetag_positions.size());

  for (size_t n = 0; n < referencetag_positions.size(); ++n) {
    reference_tag_positions_[n] = referencetag_positions[n].head<2>();
  }
}

// //-----------------------------------------------------------------------------
// void RTLSPosition2DEstimator::loadGeometry(
//   const VectorOfEigenVector3d & referencetag_positions)
// {
//   ranges_.resize(referencetag_positions.size());
//   reference_tag_positions_.resize(referencetag_positions.size());

//   for (size_t n = 0; n < referencetag_positions.size(); ++n) {
//     reference_tag_positions_[n] = referencetag_positions[n].head<2>();
//   }
// }

//--------------------------------------- --------------------------------------
bool RTLSPosition2DEstimator::init(const RangeVector & ranges)
{
  if (ranges_.empty()) {
    return false;
  }

  indexes_of_available_ranges_.clear();
  for (size_t n = 0; n < ranges.size(); ++n) {
    if (ranges[n].has_value()) {
      indexes_of_available_ranges_.push_back(n);
      ranges_[n] = ranges[n].value();
    }
  }

  if (
    indexes_of_available_ranges_.size() == ranges_.size() ||
    indexes_of_available_ranges_.size() > MINIMAL_NUMBER_OF_RANGES_TO_COMPUTE_POSITION) {
    leastSquares_.setDataSize(indexes_of_available_ranges_.size());
    return true;
  } else {
    return false;
  }
}

// //---------------------------------------
// -------------------------------------- bool
// RTLSPosition2DEstimator::init(const RTLSLocalisationRangeVector & ranges)
// {
//   if (ranges_.empty()) {
//     return false;
//   }

//   indexes_of_available_ranges_.clear();
//   for (size_t n = 0; n < ranges.size(); ++n) {
//     if (ranges[n].isAvailable()) {
//       indexes_of_available_ranges_.push_back(n);
//       ranges_[n] = ranges[n].computeUnbiasedRange2D();
//     }
//   }

//   if (indexes_of_available_ranges_.size() == ranges_.size() ||
//     indexes_of_available_ranges_.size() >
//     MINIMAL_NUMBER_OF_RANGES_TO_COMPUTE_POSITION)
//   {
//     leastSquares_.setDataSize(indexes_of_available_ranges_.size());
//     return true;
//   } else {
//     return false;
//   }
// }

//-----------------------------------------------------------------------------
void RTLSPosition2DEstimator::computeGuess_()
{
  estimate_ =
    SimpleTrilateration2D::compute(reference_tag_positions_, ranges_, indexes_of_available_ranges_);
}

//-----------------------------------------------------------------------------
void RTLSPosition2DEstimator::computeJacobianAndY_()
{
  auto & J = leastSquares_.getJ();
  auto & Y = leastSquares_.getY();

  for (size_t n = 0; n < indexes_of_available_ranges_.size(); ++n) {
    size_t rangeIndex = indexes_of_available_ranges_[n];
    double dx = estimate_(0) - reference_tag_positions_[rangeIndex].x();
    double dy = estimate_(1) - reference_tag_positions_[rangeIndex].y();

    Y(static_cast<int>(n)) = std::sqrt(dx * dx + dy * dy);

    J.row(static_cast<int>(n)) << dx, dy;
    J.row(static_cast<int>(n)) /= Y(static_cast<int>(n));

    Y(static_cast<int>(n)) -= ranges_[rangeIndex];
  }
}

}  // namespace core
}  // namespace romea
