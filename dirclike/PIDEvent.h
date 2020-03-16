#ifndef __PIDEvent_H__
#define __PIDEvent_H__

#include <vector>

class PIDPhoton
{
 public:
  double x;
  double y;
  double z;
  double Phi;
  double Angle;
  double lPro;
  double tPro;
  double tGen;
  double tTot;
  int SensorID;

 public:
  PIDPhoton();
  ~PIDPhoton() {}
  PIDPhoton(const PIDPhoton& right);
  PIDPhoton &operator=(const PIDPhoton &right);

  ClassDef(PIDPhoton, 1);
};

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

  double    CherenkovAngle;
  double    P;
  double    Mass;
  double    dNdx;
  int       Npe;
  int       nPhotons;

  vector<PIDPhoton*> fPhotons;

  void SetCherenkovAngle(double value) { CherenkovAngle = value; }
  void SetMomentum(double value) { P = value; }
  void SetMass(double value) { Mass = value; }
  void SetdNdx(double value) { dNdx = value; }
  void SetNpe(double value) { Npe = value; }
  void SetNPhotons(int value) { nPhotons = value; }
  int GetNPhotons() { return nPhotons; }
  void AddPhoton(PIDPhoton &ph);
  PIDPhoton* GetPhotons();

  ClassDef(PIDEvent, 1);

};

#endif /* __PIDEvent_H__ */
