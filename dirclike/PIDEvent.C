#include <iostream>

#include "PIDPhoton.h"
#include "PIDEvent.h"

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

PIDEvent::~PIDEvent()
{
  nPhotons = 0;

  /*
  for(int i=0; i<(int)fPhotons.size(); i++)
    {
      PIDPhoton* phts = fPhotons.at(i);
      delete phts;
      phts = NULL;
    }
  */
}

void PIDEvent::AddPhoton(PIDPhoton &ph)
{
  fPhotons.push_back( ph );
  nPhotons++;
}

PIDEvent& PIDEvent::operator=(const PIDEvent &right)
{
  (*this) = right;
  return *this;
}

PIDPhoton* PIDEvent::GetPhotons()
{

  if( nPhotons == 0 )
    return NULL;

  PIDPhoton* phs = new PIDPhoton[nPhotons];
  for(int iph=0; iph<nPhotons; iph++)
    {
      phs[iph].x = fPhotons[iph].x;
      phs[iph].y = fPhotons[iph].y;
      phs[iph].z = fPhotons[iph].z;
      phs[iph].Phi = fPhotons[iph].Phi;
      phs[iph].Angle = fPhotons[iph].Angle;
      phs[iph].lPro = fPhotons[iph].lPro;
      phs[iph].tPro = fPhotons[iph].tPro;
      phs[iph].tGen = fPhotons[iph].tGen;
      phs[iph].tTot = fPhotons[iph].tTot;
      phs[iph].SensorID = fPhotons[iph].SensorID;
    }

  return phs;
}
