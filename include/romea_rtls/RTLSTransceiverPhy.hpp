#ifndef _romea_RTLSTransceiverPhy_hpp_
#define _romea_RTLSTransceiverPhy_hpp_

namespace romea{

class RTLSTransceiverPhy
{

public :

  RTLSTransceiverPhy(const double &centreFrequency,
                     const double &dataRate,
                     const double &bandWidth,
                     const int & prf,
                     const double & minimal_range,
                     const double & maximal_range,
                     const int &userPayloadMaximalLength);

public :


  const int & getUserPayloadMaximalLength()const;

  const double & getCentreFrequency()const;

  const double & getDataRate()const;

  const double & getBandWidth()const;

  const int & getPRF()const;

  const double & getMinimalRange()const;

  const double & getMaximalRange()const;

private :

  double centreFrequency_;
  double dataRate_;
  double bandWidth_;
  int prf_;
  double minimal_range_;
  double maximal_range_;
  int userPayloadMaximalLength_;

};

bool operator==(const RTLSTransceiverPhy & phy1,const RTLSTransceiverPhy & phy2);

bool operator!=(const RTLSTransceiverPhy & phy1,const RTLSTransceiverPhy & phy2);

}

#endif
