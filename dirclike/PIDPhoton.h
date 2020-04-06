#ifndef __PIDPhoton_H__
#define __PIDPhoton_H__

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
};

#endif /* __PIDPhoton_H__ */
