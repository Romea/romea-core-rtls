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


#ifndef ROMEA_CORE_RTLS__RTLSRANGERANDOMNOISE_HPP
#define ROMEA_CORE_RTLS__RTLSRANGERANDOMNOISE_HPP

// std
#include <random>

// romea
#include "romea_core_rtls/RTLSRangeNoise.hpp"


namespace romea
{

class RTLSRangeRandomNoise : public RTLSRangeNoise
{
public:
  RTLSRangeRandomNoise();

  RTLSRangeRandomNoise(
    const double & a,
    const double & b);

public:
  double draw(const double & range);

private:
  std::default_random_engine generator_;
  std::normal_distribution<double> dist_;
};

}  // namespace romea

#endif  // ROMEA_CORE_RTLS__RTLSRANGERANDOMNOISE_HPP
