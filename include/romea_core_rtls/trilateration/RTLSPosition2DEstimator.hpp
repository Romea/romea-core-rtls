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

#ifndef ROMEA_CORE_RTLS__TRILATERATION__RTLSPOSITION2DESTIMATOR_HPP_
#define ROMEA_CORE_RTLS__TRILATERATION__RTLSPOSITION2DESTIMATOR_HPP_

// std
#include <optional>
#include <vector>

// romea
#include "romea_core_common/regression/leastsquares/NLSE.hpp"
#include "romea_core_common/containers/Eigen/VectorOfEigenVector.hpp"

namespace romea
{
namespace core
{

class RTLSPosition2DEstimator : public NLSE<double>
{
public:
  using RangeVector = std::vector<std::optional<double>>;

public:
  RTLSPosition2DEstimator(
    const VectorOfEigenVector3d & referenceTagPosition,
    const double & estimateEpsilon = 0.01);

  bool init(const RangeVector & ranges);

private:
  void computeGuess_()override;

  void computeJacobianAndY_()override;

private:
  VectorOfEigenVector2d referenceTagPositions_;
  std::vector<size_t> indexesOfAvailableRanges_;
  std::vector<double> ranges_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_RTLS__TRILATERATION__RTLSPOSITION2DESTIMATOR_HPP_
