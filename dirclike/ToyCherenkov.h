#ifndef __TOYCHERENKOV_H__
#define __TOYCHERENKOV_H__

#include <string>

#include "PIDEvent.h"

class ToyCherenkov
{
 public:
  ToyCherenkov(int pid);
  virtual ~ToyCherenkov();
  void SetDetector(double n, double l, double t, double tilt_angle);
  void SetCutoff(double value);
  void SetdT(double value);
  void SetVerbosity(int value){ verbosity = value; }

  int  run(double p, PIDEvent* event);

 private:
  double fn;
  double fLength;
  double fThickness;
  double fAngle;
  double fCutoff;
  double sigmaT;

  int verbosity;

  double beta;
  double mass;
  double thetac;
  std::string particlename;

  double getdNdx(double p);
  double getIntegralQE(double cutoff);
  double getCerAngle(double p);

};

#endif /*__TOYCHERENKOV_H__*/
