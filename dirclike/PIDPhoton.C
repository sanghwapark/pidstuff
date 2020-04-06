#include "PIDPhoton.h"

PIDPhoton::PIDPhoton()
{
  x     = -999;
  y     = -999;
  z     = -999;
  Phi   = -999;
  Angle = -999;   
  lPro  = -999;
  tPro  = -999;
  tGen  = -999;
  tTot  = -999;
  SensorID = -999;
}

PIDPhoton::PIDPhoton(const PIDPhoton &right)
{
  x     = right.x;
  y     = right.y;
  z     = right.z;
  Phi   = right.Phi;
  Angle = right.Angle;
  lPro  = right.lPro;
  tPro  = right.tPro;
  tGen  = right.tGen;
  tTot  = right.tTot;
  SensorID = right.SensorID;
}

PIDPhoton& PIDPhoton::operator=(const PIDPhoton &right)
{
  (*this) = right;
  return *this;
}
