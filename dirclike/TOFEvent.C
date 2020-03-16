TOFEvent::TOFEvent()
{

}

TOFEvent::~TOFEvent()
{

}


Photons::Photons()
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

Photons::Photons(const Photons &right)
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

const Photons& Photons::operator =(const Photons &right)
{
  (*this) = right;
  return this;
}

