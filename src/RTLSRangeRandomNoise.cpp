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


#include "romea_core_rtls/RTLSRangeRandomNoise.hpp"

namespace romea
{

//-----------------------------------------------------------------------------
RTLSRangeRandomNoise::RTLSRangeRandomNoise()
: RTLSRangeNoise(0, 0),
  generator_(std::random_device{} ()),
  dist_(0, 1)
{
}

//-----------------------------------------------------------------------------
RTLSRangeRandomNoise::RTLSRangeRandomNoise(
  const double & a,
  const double & b)
: RTLSRangeNoise(a, b)
{
}

//-----------------------------------------------------------------------------
double RTLSRangeRandomNoise::draw(const double & range)
{
  return dist_(generator_) * computeStd(range);
}

}  // namespace romea
