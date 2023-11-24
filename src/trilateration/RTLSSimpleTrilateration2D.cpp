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
#include <algorithm>
#include <utility>
#include <vector>

// romea
#include "romea_core_rtls/trilateration/RTLSSimpleTrilateration2D.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
VectorOfEigenVector2d SimpleTrilateration2D::compute_(
  const Eigen::Vector2d & p1,
  const Eigen::Vector2d & p2,
  const double & r1,
  const double & r2)
{
  double dx = p2.x() - p1.x();
  double dy = p2.y() - p1.y();

  double base = std::sqrt(dx * dx + dy * dy);
  double theta = std::atan2(dy, dx);
  //  std::cout << (base*base+r1*r1-r2*r2)/(2*base*r1) << std::endl;
  double alpha =
    std::acos(std::max(std::min((base * base + r1 * r1 - r2 * r2) / (2 * base * r1), 1.), -1.));

  VectorOfEigenVector2d solutions(2, p1);
  solutions[0].x() += r1 * std::cos(theta + alpha);
  solutions[0].y() += r1 * std::sin(theta + alpha);
  solutions[1].x() += r1 * std::cos(theta - alpha);
  solutions[1].y() += r1 * std::sin(theta - alpha);

  if (solutions[0].x() < 0) {
    std::swap(solutions[0], solutions[1]);
  }

  //  std::cout << " p1 "<<p1.transpose()<< std::endl;
  //  std::cout << " p2 "<<p2.transpose()<< std::endl;
  //  std::cout << " r1 "<<r1<< std::endl;
  //  std::cout << " r2 "<<r2<< std::endl;

  //  std::cout << " base "<<base<< std::endl;
  //  std::cout << " theta "<<theta*180/M_PI<< std::endl;
  //  std::cout << " alpha "<<alpha*180/M_PI<< std::endl;

  //  std::cout << " solutions[0] "<<solutions[0].transpose()<< std::endl;
  //  std::cout << " solutions[1] "<<solutions[1].transpose()<< std::endl;

  return solutions;
}


//-----------------------------------------------------------------------------
Eigen::Vector2d SimpleTrilateration2D::compute_(
  const VectorOfEigenVector2d & tagPositions,
  const std::vector<double> & ranges,
  const size_t & i,
  const size_t & j)
{
  std::vector<double> errors(2, 0);

  VectorOfEigenVector2d solutions = compute_(
    tagPositions[i],
    tagPositions[j],
    ranges[i],
    ranges[j]);
  size_t k = (j + 1) % ranges.size();
  for (; k != i; k = (k + 1) % ranges.size()) {
    errors[0] += std::abs((tagPositions[k] - solutions[0]).norm() - ranges[k]);
    errors[1] += std::abs((tagPositions[k] - solutions[1]).norm() - ranges[k]);
  }

//  std::cout <<" errors "<< errors[0] <<" "<<errors[1]<< std::endl;

  return errors[0] <= errors[1] ? solutions[0] : solutions[1];
}


//-----------------------------------------------------------------------------
Eigen::Vector2d SimpleTrilateration2D::compute_(
  const VectorOfEigenVector2d & tagPositions,
  const std::vector<double> & ranges,
  const std::vector<size_t> & rangesIndexes,
  const size_t & i,
  const size_t & j)
{
  std::vector<double> errors(2, 0);

  VectorOfEigenVector2d solutions = compute_(
    tagPositions[i],
    tagPositions[j],
    ranges[rangesIndexes[i]],
    ranges[rangesIndexes[j]]);

  size_t k = (j + 1) % ranges.size();
  for (; k != i; k = (k + 1) % ranges.size()) {
    errors[0] += std::abs((tagPositions[k] - solutions[0]).norm() - ranges[rangesIndexes[k]]);
    errors[1] += std::abs((tagPositions[k] - solutions[1]).norm() - ranges[rangesIndexes[k]]);
  }

  //  std::cout <<" errors "<< errors[0] <<" "<<errors[1]<< std::endl;
  return errors[0] <= errors[1] ? solutions[0] : solutions[1];
}

//-----------------------------------------------------------------------------
Eigen::Vector2d SimpleTrilateration2D::compute(
  const VectorOfEigenVector2d & tagPositions,
  const std::vector<double> & ranges)
{
  assert(tagPositions.size() >= 2);
  assert(tagPositions.size() == ranges.size());

  if (tagPositions.size() == 2) {
    return compute_(tagPositions, ranges, 0, 1);
  } else {
    Eigen::Vector2d solution = Eigen::Vector2d::Zero();
    for (size_t i = 0, j = 1; i < ranges.size(); ++i, j = (j + 1) % ranges.size()) {
      Eigen::Vector2d solution_ = compute_(tagPositions, ranges, i, j);
      solution += solution_;
    }
    return solution / ranges.size();
  }
}

//-----------------------------------------------------------------------------
Eigen::Vector2d SimpleTrilateration2D::compute(
  const VectorOfEigenVector2d & tagPositions,
  const std::vector<double> & ranges,
  const std::vector<size_t> & rangesIndexes)
{
  assert(tagPositions.size() >= 2);
  assert(rangesIndexes.size() >= 2);
  assert(tagPositions.size() == ranges.size());

  if (tagPositions.size() == 2) {
    return compute_(tagPositions, ranges, rangesIndexes, 0, 1);
  } else {
    Eigen::Vector2d solution = Eigen::Vector2d::Zero();
    for (size_t i = 0, j = 1; i < rangesIndexes.size(); ++i, j = (j + 1) % rangesIndexes.size()) {
      solution += compute_(
        tagPositions,
        ranges,
        rangesIndexes,
        i,
        j);
    }
    return solution / ranges.size();
  }
}

}  // namespace core
}  // namespace romea
