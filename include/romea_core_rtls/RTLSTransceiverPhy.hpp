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


#ifndef ROMEA_CORE_RTLS__RTLSTRANSCEIVERPHY_HPP_
#define ROMEA_CORE_RTLS__RTLSTRANSCEIVERPHY_HPP_

namespace romea
{

class RTLSTransceiverPhy
{
public:
  RTLSTransceiverPhy(
    const double & centreFrequency,
    const double & dataRate,
    const double & bandWidth,
    const int & prf,
    const double & minimal_range,
    const double & maximal_range,
    const int & userPayloadMaximalLength);

public:
  const int & getUserPayloadMaximalLength()const;

  const double & getCentreFrequency()const;

  const double & getDataRate()const;

  const double & getBandWidth()const;

  const int & getPRF()const;

  const double & getMinimalRange()const;

  const double & getMaximalRange()const;

private:
  double centreFrequency_;
  double dataRate_;
  double bandWidth_;
  int prf_;
  double minimal_range_;
  double maximal_range_;
  int userPayloadMaximalLength_;
};

bool operator==(const RTLSTransceiverPhy & phy1, const RTLSTransceiverPhy & phy2);

bool operator!=(const RTLSTransceiverPhy & phy1, const RTLSTransceiverPhy & phy2);

}  // namespace romea

#endif  // ROMEA_CORE_RTLS__RTLSTRANSCEIVERPHY_HPP_
