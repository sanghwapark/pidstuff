#include "PIDEvent.h"

ClassImp(PIDPhoton);
ClassImp(PIDEvent);

PIDEvent::PIDEvent()
{
  SetRadiator(0., 0., 0.);
  SetAngle(0.);
  SetLambdaCutoff(0.);
  SetSigmaT(0.);

  CherenkovAngle = -999;
  P = -999;
  Mass = -999;
  dNdx = -999;
  Npe = -999;

  nPhotons = 0;
  fPhotons.clear();
}

void PIDEvent::AddPhoton(PIDPhoton &ph)
{
  fPhotons.push_back( new PIDPhoton(ph) );
  nPhotons++;
}

PIDPhoton* PIDEvent::GetPhotons()
{

  if( nPhotons == 0 )
    return NULL;

  PIDPhoton* phs = new PIDPhoton[nPhotons];
  for(int iph=0; iph<nPhotons; iph++)
    {
      phs[iph].x = fPhotons[iph]->x;
      phs[iph].y = fPhotons[iph]->y;
      phs[iph].z = fPhotons[iph]->z;
      phs[iph].Phi = fPhotons[iph]->Phi;
      phs[iph].Angle = fPhotons[iph]->Angle;
      phs[iph].lPro = fPhotons[iph]->lPro;
      phs[iph].tPro = fPhotons[iph]->tPro;
      phs[iph].tGen = fPhotons[iph]->tGen;
      phs[iph].tTot = fPhotons[iph]->tTot;
      phs[iph].SensorID = fPhotons[iph]->SensorID;
    }

  return phs;

}

PIDEvent& PIDEvent::operator=(const PIDEvent &right)
{
  (*this) = right;
  return *this;
}


PIDEvent::~PIDEvent()
{
  nPhotons = 0;
}

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

