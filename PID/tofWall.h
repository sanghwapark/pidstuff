#ifndef __TOFWALL_H__
#define __TOFWALL_H__
	
//
//
//  This is an example class that inherits from the PID base class.
//  We shall implement all the necessary functions as required by the
//  base class.
//
//  The UNIQUE identifiers for this class are rmin (inner), rmax (outer),
//  zposition and time resolution. 
//
//  The inner and outer radius and z positiion define eta range
//
//  Note that in keeping with any well written base class, we shall
//  give default values to all the constructor arguments.
//
//  This routine assumes units of cm for distances and picoSeconds for time.
//

#include "PID.h"
	
class tofWall: public PID
{
public:
  tofWall(double rmin=1, double rmax=100, double z=200, double sigmaT=10);
  virtual ~tofWall() {}
	
  bool   valid   (double eta, double p) {return (eta>etaLow && eta<etaHigh);}
  double numSigma(double eta, double p,        PID::type PID);
  double maxP    (double eta, double numSigma, PID::type PID);
  double minP    (double eta, double numSigma, PID::type PID) {return 0;}
  string name    () {return myName;}
  void   description ();
		
protected:
  std::string myName;

  // utility function
  double tof(double L, double p, double m);

  // TOF wall parameters
  double radiusIn;   // cm
  double radiusOut;   // cm
  double zposition;   // cm
  double etaLow;
  double etaHigh;
  double sigmaT;   // picosecond
};
	
#endif /* __PID_H__ */
