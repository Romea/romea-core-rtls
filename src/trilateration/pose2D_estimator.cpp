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
#include <vector>

// romea
#include "romea_core_common/math/EulerAngles.hpp"
#include "romea_core_common/transform/estimation/FindRigidTransformationBySVD.hpp"
#include "romea_core_rtls/trilateration/pose2D_estimator.hpp"
#include "romea_core_rtls/trilateration/simple_trilateration2D.hpp"

namespace {
const size_t MINIMAL_NUMBER_OF_TAGS_ON_TARGET_ENTITY = 2;
const size_t MINIMAL_NUMBER_OF_TAGS_ON_REFERENCE_ENTITY = 3;
const size_t MINIMAL_NUMBER_OF_RANGES_TO_COMPUTE_POSITION = 3;
const size_t MINIMAL_NUMBER_OF_RANGES_FROM_EACH_TAG_TO_COMPUTE_POSE = 2;
}  // namespace

namespace romea {
namespace core {

//-----------------------------------------------------------------------------
RTLSPose2DEstimator::RTLSPose2DEstimator(
    const VectorOfEigenVector3d& targettag_positions,
    const VectorOfEigenVector3d& referencetag_positions,
    const double& estimateEpsilon)
    : NLSE(estimateEpsilon) {
  estimate_.resize(3);
  estimateCovariance_.resize(3, 3);
  leastSquares_.setEstimateSize(3);

  ranges_.resize(targettag_positions.size(),
                 std::vector<double>(referencetag_positions.size()));

  indexes_of_available_ranges_.resize(
      targettag_positions.size(),
      std::vector<size_t>(referencetag_positions.size()));

  target_tag_positions_.resize(targettag_positions.size());
  for (size_t i = 0; i < targettag_positions.size(); ++i) {
    target_tag_positions_[i] = targettag_positions[i].head<2>();
  }

  reference_tag_positions_.resize(referencetag_positions.size());
  for (size_t j = 0; j < referencetag_positions.size(); ++j) {
    reference_tag_positions_[j] = referencetag_positions[j].head<2>();
  }
}

// //-----------------------------------------------------------------------------
// void RTLSPose2DEstimator::loadGeometry(
//   const VectorOfEigenVector3d & targettag_positions,
//   const VectorOfEigenVector3d & referencetag_positions)
// {
//   ranges_.resize(
//     targettag_positions.size(),
//     std::vector<double>(referencetag_positions.size()));

//   indexes_of_available_ranges_.resize(
//     targettag_positions.size(),
//     std::vector<size_t>(referencetag_positions.size()));

//   target_tag_positions_.resize(targettag_positions.size());
//   for (size_t i = 0; i < targettag_positions.size(); ++i) {
//     target_tag_positions_[i] = targettag_positions[i].head<2>();
//   }

//   reference_tag_positions_.resize(referencetag_positions.size());
//   for (size_t j = 0; j < referencetag_positions.size(); ++j) {
//     reference_tag_positions_[j] = referencetag_positions[j].head<2>();
//   }
// }

//-----------------------------------------------------------------------------
bool RTLSPose2DEstimator::init(const RangeArray& ranges) {
  const size_t numberOfReferenceTags = reference_tag_positions_.size();
  const size_t numberOfTargetTags = target_tag_positions_.size();

  assert(ranges.size() == numberOfTargetTags);
  assert(ranges[0].size() == numberOfReferenceTags);

  size_t n = 0;
  for (size_t i = 0; i < numberOfTargetTags; i++) {
    indexes_of_available_ranges_[i].clear();
    for (size_t j = 0; j < numberOfReferenceTags; j++) {
      const auto& range = ranges[i][j];
      if (range.has_value()) {
        indexes_of_available_ranges_[i].push_back(j);
        ranges_[i][j] = range.value();
        n++;
      }
    }

    if (indexes_of_available_ranges_[i].size() != ranges_[i].size() &&
        indexes_of_available_ranges_[i].size() <
            MINIMAL_NUMBER_OF_RANGES_TO_COMPUTE_POSITION) {
      return false;
    }
  }

  leastSquares_.setDataSize(n);
  return true;
}

// //-----------------------------------------------------------------------------
// bool RTLSPose2DEstimator::initR2R(const RTLSLocalisationRangeArray & ranges)
// {
//   const size_t numberOfReferenceTags = reference_tag_positions_.size();
//   const size_t numberOfTargetTags = target_tag_positions_.size();

//   assert(ranges.size() == numberOfTargetTags);
//   assert(ranges[0].size() == numberOfReferenceTags);

//   size_t n = 0;
//   for (size_t i = 0; i < numberOfTargetTags; i++) {
//     indexes_of_available_ranges_[i].clear();
//     for (size_t j = 0; j < numberOfReferenceTags; j++) {
//       auto & range = ranges[i][j];
//       if (range.isAvailable()) {
//         indexes_of_available_ranges_[i].push_back(j);
//         ranges_[i][j] = range.computeUnbiasedRange2D();
//         n++;
//       }
//     }

//     if (indexes_of_available_ranges_[i].size() != ranges_[i].size() &&
//       indexes_of_available_ranges_[i].size() <
//       MINIMAL_NUMBER_OF_RANGES_TO_COMPUTE_POSITION)
//     {
//       return false;
//     }
//   }

//   leastSquares_.setDataSize(n);
//   return true;
// }

// //-----------------------------------------------------------------------------
// bool RTLSPose2DEstimator::initR2W(
//   const RTLSLocalisationRangeArray & ranges,
//   const std::vector<size_t> & referenceTagIndexes)
// {
//   const size_t numberOfReferenceTags = reference_tag_positions_.size();
//   const size_t numberOfTargetTags = target_tag_positions_.size();

//   assert(ranges.size() == numberOfReferenceTags);
//   assert(ranges[0].size() == numberOfTargetTags);

//   size_t n = 0;
//   for (size_t i = 0; i < numberOfTargetTags; i++) {
//     indexes_of_available_ranges_[i].clear();
//     for (size_t j = 0; j < referenceTagIndexes.size(); j++) {
//       auto & range = ranges[referenceTagIndexes[j]][i];
//       if (range.isAvailable()) {
//         indexes_of_available_ranges_[i].push_back(j);
//         ranges_[i][j] = range.computeUnbiasedRange2D();
//         n++;
//       }
//     }

//     if (indexes_of_available_ranges_[i].size() != ranges_[i].size() &&
//       indexes_of_available_ranges_[i].size() <
//       MINIMAL_NUMBER_OF_RANGES_TO_COMPUTE_POSITION)
//     {
//       return false;
//     }
//   }

//   leastSquares_.setDataSize(n);
//   return true;
// }

//-----------------------------------------------------------------------------
void RTLSPose2DEstimator::computeGuess_() {
  VectorOfEigenVector2d targetTagGuessPositions(target_tag_positions_.size());

  for (size_t i = 0; i < target_tag_positions_.size(); i++) {
    targetTagGuessPositions[i] = SimpleTrilateration2D::compute(
        reference_tag_positions_, ranges_[i], indexes_of_available_ranges_[i]);
  }

  FindRigidTransformationBySVD<Eigen::Vector2d> estimator_;
  Eigen::Matrix3d H =
      estimator_.find(target_tag_positions_, targetTagGuessPositions);
  estimate_(0) = H(0, 2);
  estimate_(1) = H(1, 2);
  estimate_(2) = rotation2DToEulerAngle<double>(H.block(0, 0, 2, 2));

  //  std::cout<<" guess estimate_ " << std::endl;
  //  std::cout<< estimate_.transpose() << std::endl;
}

//-----------------------------------------------------------------------------
void RTLSPose2DEstimator::computeJacobianAndY_() {
  auto& J = leastSquares_.getJ();
  auto& Y = leastSquares_.getY();

  size_t n = 0;
  for (size_t i = 0; i < target_tag_positions_.size(); i++) {
    for (const size_t& j : indexes_of_available_ranges_[i]) {
      double x = estimate_(0);
      double y = estimate_(1);
      double o = estimate_(2);

      double coso = std::cos(o);
      double sino = std::sin(o);

      double xt = target_tag_positions_[i].x();
      double yt = target_tag_positions_[i].y();

      double xr = reference_tag_positions_[j].x();
      double yr = reference_tag_positions_[j].y();

      double alpha = x + xt * coso - yt * sino - xr;
      double gamma = y + xt * sino + yt * coso - yr;
      Y(static_cast<int>(n)) = std::sqrt(alpha * alpha + gamma * gamma);

      J.row(static_cast<int>(n)) << alpha, gamma,
          alpha * (-xt * sino - yt * coso) + gamma * (xt * coso - yt * sino);
      J.row(static_cast<int>(n)) /= Y(static_cast<int>(n));

      Y(static_cast<int>(n)) -= ranges_[i][j];
      n++;
    }
  }

  //  std::cout <<" J "<< std::endl;
  //  std::cout << J << std::endl;

  //  std::cout << "Y" << std::endl;
  //  std::cout << Y << std::endl;
}

}  // namespace core
}  // namespace romea
