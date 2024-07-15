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

// std
#include <vector>

// gtest
#include "gtest/gtest.h"

// romea
#include "romea_core_common/math/EulerAngles.hpp"
#include "romea_core_rtls/trilateration/RTLSPose2DEstimator.hpp"

//-----------------------------------------------------------------------------
TEST(TestRtlsPoseEstimator, testRtlsPoseEstimator)
{
  using RangeVector = std::vector<std::optional<double>>;
  using RangeArray = std::vector<RangeVector>;

  romea::core::VectorOfEigenVector3d tagPositions;
  tagPositions.emplace_back(0, -0.3, 1.01);
  tagPositions.emplace_back(0, 0.3, 1.01);
  tagPositions.emplace_back(0.44, 0, 0.71);

  romea::core::VectorOfEigenVector3d anchorPositions;
  anchorPositions.emplace_back(0, -0.21, 0.39);
  anchorPositions.emplace_back(0, 0.21, 0.39);
  anchorPositions.emplace_back(0.85, 0, 0.44);

  romea::core::RTLSPose2DEstimator estimator(anchorPositions, tagPositions, 0.001);

  double rho = 5;
  double theta = 0;
  double course = -M_PI;
  for (; theta < 2 * M_PI; theta += M_PI / 4, course += M_PI / 3) {
    Eigen::Matrix3d R = romea::core::eulerAnglesToRotation3D(
      Eigen::Vector3d(0, 0, romea::core::between0And2Pi(course)));
    Eigen::Vector3d T(rho * std::cos(theta), rho * std::cos(theta), 0);

    RangeArray ranges(3, RangeVector(3));
    for (size_t i = 0; i < 3; i++) {
      for (size_t j = 0; j < 3; j++) {
        ranges[i][j] = ((R * anchorPositions[i] + T) - tagPositions[j]).head<2>().norm();
      }
    }

    EXPECT_TRUE(estimator.init(ranges));
    EXPECT_TRUE(estimator.estimate(10, 0.02));
    EXPECT_NEAR(T[0], estimator.getEstimate()[0], 0.01);
    EXPECT_NEAR(T[1], estimator.getEstimate()[1], 0.01);
    EXPECT_NEAR(romea::core::betweenMinusPiAndPi(course - estimator.getEstimate()[2]), 0.0, 0.01);
  }
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
