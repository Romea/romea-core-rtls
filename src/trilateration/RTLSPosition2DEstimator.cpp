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

#include "romea_core_rtls/trilateration/RTLSPosition2DEstimator.hpp"
#include "romea_core_rtls/trilateration/RTLSSimpleTrilateration2D.hpp"

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
  const VectorOfEigenVector3d & referenceTagPositions,
  const double & estimateEpsilon)
: NLSE(estimateEpsilon)
{
  estimate_.resize(2);
  estimateCovariance_.resize(2, 2);
  leastSquares_.setEstimateSize(2);

  ranges_.resize(referenceTagPositions.size());
  referenceTagPositions_.resize(referenceTagPositions.size());

  for (size_t n = 0; n < referenceTagPositions.size(); ++n) {
    referenceTagPositions_[n] = referenceTagPositions[n].head<2>();
  }
}

// //-----------------------------------------------------------------------------
// void RTLSPosition2DEstimator::loadGeometry(
//   const VectorOfEigenVector3d & referenceTagPositions)
// {
//   ranges_.resize(referenceTagPositions.size());
//   referenceTagPositions_.resize(referenceTagPositions.size());

//   for (size_t n = 0; n < referenceTagPositions.size(); ++n) {
//     referenceTagPositions_[n] = referenceTagPositions[n].head<2>();
//   }
// }

//--------------------------------------- --------------------------------------
bool RTLSPosition2DEstimator::init(const RangeVector & ranges)
{
  if (ranges_.empty()) {
    return false;
  }

  indexesOfAvailableRanges_.clear();
  for (size_t n = 0; n < ranges.size(); ++n) {
    if (ranges[n].has_value()) {
      indexesOfAvailableRanges_.push_back(n);
      ranges_[n] = ranges[n].value();
    }
  }

  if (indexesOfAvailableRanges_.size() == ranges_.size() ||
    indexesOfAvailableRanges_.size() > MINIMAL_NUMBER_OF_RANGES_TO_COMPUTE_POSITION)
  {
    leastSquares_.setDataSize(indexesOfAvailableRanges_.size());
    return true;
  } else {
    return false;
  }
}

// //--------------------------------------- --------------------------------------
// bool RTLSPosition2DEstimator::init(const RTLSLocalisationRangeVector & ranges)
// {
//   if (ranges_.empty()) {
//     return false;
//   }

//   indexesOfAvailableRanges_.clear();
//   for (size_t n = 0; n < ranges.size(); ++n) {
//     if (ranges[n].isAvailable()) {
//       indexesOfAvailableRanges_.push_back(n);
//       ranges_[n] = ranges[n].computeUnbiasedRange2D();
//     }
//   }

//   if (indexesOfAvailableRanges_.size() == ranges_.size() ||
//     indexesOfAvailableRanges_.size() > MINIMAL_NUMBER_OF_RANGES_TO_COMPUTE_POSITION)
//   {
//     leastSquares_.setDataSize(indexesOfAvailableRanges_.size());
//     return true;
//   } else {
//     return false;
//   }
// }

//-----------------------------------------------------------------------------
void RTLSPosition2DEstimator::computeGuess_()
{
  estimate_ = SimpleTrilateration2D::
    compute(referenceTagPositions_, ranges_, indexesOfAvailableRanges_);
}

//-----------------------------------------------------------------------------
void RTLSPosition2DEstimator::computeJacobianAndY_()
{
  auto & J = leastSquares_.getJ();
  auto & Y = leastSquares_.getY();

  for (size_t n = 0; n < indexesOfAvailableRanges_.size(); ++n) {
    size_t rangeIndex = indexesOfAvailableRanges_[n];
    double dx = estimate_(0) - referenceTagPositions_[rangeIndex].x();
    double dy = estimate_(1) - referenceTagPositions_[rangeIndex].y();

    Y(static_cast<int>(n)) = std::sqrt(dx * dx + dy * dy);

    J.row(static_cast<int>(n)) << dx, dy;
    J.row(static_cast<int>(n)) /= Y(static_cast<int>(n));

    Y(static_cast<int>(n)) -= ranges_[rangeIndex];
  }
}

}  // namespace core
}  // namespace romea
