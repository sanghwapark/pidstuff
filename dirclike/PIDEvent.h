#ifndef __PIDEvent_H__
#define __PIDEvent_H__

#include <vector>

#include "PIDPhoton.h"

class PIDEvent
{
 public:
  PIDEvent();
  ~PIDEvent();

  double RefractiveIndex;
  double Length;
  double Thickness;
  double Angle;
  double LambdaCutoff;
  double SigmaT;

  void SetRadiator(double n, double l, double t) { RefractiveIndex = n; Length = l; Thickness = t; }
  void SetAngle(double a) { Angle = a; }
  void SetLambdaCutoff(double cutoff) { LambdaCutoff = cutoff; }
  void SetSigmaT(double dt) { SigmaT = dt; }
  PIDEvent &operator=(const PIDEvent &right);

  double    CherenkovAngle;
  double    P;
  double    Mass;
  double    Beta;
  double    dNdx;
  int       Npe;
  int       nPhotons;

  std::vector<PIDPhoton> fPhotons;

  void SetCherenkovAngle(double value) { CherenkovAngle = value; }
  void SetMomentum(double value) { P = value; }
  void SetMass(double value) { Mass = value; }
  void SetBeta(double value) { Beta = value; }
  void SetdNdx(double value) { dNdx = value; }
  void SetNpe(double value) { Npe = value; }
  //  void SetNPhotons(int value) { nPhotons = value; }
  int GetNPhotons() { return nPhotons; }
  void AddPhoton(PIDPhoton &ph);
  PIDPhoton* GetPhotons();

};

#endif /* __PIDEvent_H__ */
