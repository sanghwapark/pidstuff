#ifndef __TOFEVENT_H__
#define __TOFEVENT_H__

class PIDDet
{
 public:
  double RefractiveIndex;
  double Length;
  double Thickness;
  double Angle;
  double LambdaCutoff;
  double SigmaT;

 public:
  PIDDet() : RefractiveIndex(0.), Length(0.), Thickness(0.) {}
  virtual ~PIDDet() {}
  void SetRadiator(double n, double l, double t) { RefractiveIndex = n; Length = l; Thickness = t; }
  void SetAngle(double a) { Angle = a; }
  void SetLambdaCutoff(double cutoff) { LambdaCutoff = cutoff; }
  void SetSigmaT(double dt) { SigmaT = dt; }

  ClassDef(PIDDet, 1);
};

class Photons
{
 public:
  double x;
  double y;
  double z;
  double Phi;
  double Angle;
  double lPro;      // propatation length
  double tPro;      // propatation time
  double tGen;      // time when generated
  double tTot;      // Total time to detect
  int    SensorID;

 public:
  Photons() {}
  Photons(const Photons& right);
  virtual ~Photons();
  Photons &operator=(const Photons &right);

  ClassDef(Photons, 1);
};

#define __MAXPHOTONS 10000

class PIDEvent
{
 public:
  double   CherenkovAngle;
  double   P;
  double   Mass;
  double   dNdx;
  int      Npe;
  Photons  fPhotons[__MAXPHOTONS];
  int      nPhtons;

 public:
  PIDEvent() {}
  virtual ~PIDEvent() {}
  //  Photons &GetPhotons() { return fPhotons; }
  int  GetNphotons() { return nPhotons; }
  void AddPhoton(Photons* ph);
  Photons* GetPhotons() { return fPhotons; }
  
  ClassDef(PIDEvent, 1);
};

#endif /* __TOFEVENT_H__ */
