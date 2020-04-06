#include <iostream>
#include <string>

#include <TVector3.h>
#include <TRandom3.h>

#include "ToyCherenkov.h"
#include "PIDEvent.h"

/*
Coordiante system (right-hand coordinate)
^ y+
|
|
|
|
x-----------> z+

from the particle view:
^ y+
|
|
|
----------> x-
 */

// Physics constants
const double me     = 0.511e-3;       // GeV/c2
const double mpion  = 0.13957018;     // GeV/c2
const double mkaon  = 0.493677;       // GeV/c2
const double mproton= 0.93827208816;  // GeV/c2
const double c      = 0.0299792458;   // cm/ps
const double alpha  = 1./137;
const double pi     = 3.14159265;
const double cm2m   = 1.e-2;          // cm to m

enum {ELECTRON, PION, KAON, PROTON};
std::string roname[3] = {"TOP", "BOTTOM", "NO SIGNAL"};

using namespace std;

ToyCherenkov::ToyCherenkov(int pid)
  :fn(1.02),
   fLength(20.),
   fThickness(2.),
   fAngle(-10.),
   fCutoff(300.),
   sigmaT(10.)
{

  verbosity = 0;

  // Set particle mass
  if( pid == ELECTRON )
    {
      mass = me;
      particlename = "Electron";
      cout << "Particle set to " << particlename.c_str() << endl;
    }
  else if( pid == PION )
    {
      mass = mpion;
      particlename = "Pion";
      cout << "Particle set to " << particlename.c_str() << endl;
    }
  else if( pid == KAON )
    {
      mass = mkaon;
      particlename = "Kaon";
      cout << "Particle set to " << particlename.c_str() << endl;
    }
  else if( pid == PROTON )
    {
      mass = mproton;
      particlename = "Proton";
      cout << "Particle set to " << particlename.c_str() << endl;
    }
  else
    {
      cout << "Invalid particle selection" << endl;
      return;
    }

}

ToyCherenkov::~ToyCherenkov()
{

}


int ToyCherenkov::run(double p, PIDEvent* event)
{

  // calculate beta and Cherenkov angle
  beta = sqrt( pow(p/mass,2) / (1. + pow(p/mass,2)) );
  thetac = (fn*beta > 1) ? acos(1./fn/beta) : 0;

  if(thetac == 0)
    {
      cout << "No Cherenkov radiation: " << particlename.c_str()
	   << " p = " << p << "GeV/c, n = " << fn << endl;
      return -1;
    }

  // Initial coordinate center
  double x0 = 0;
  double y0 = 0;
  double z0 = 0;

  //Incident direction
  TVector3 dir_vec(0,0,1);

  TRandom3 rand(0);

  double dndx = getdNdx(p);
  double ltravel = fThickness / cos(fAngle * pi / 180.); // cm
  double npe = dndx * ltravel * cm2m;

  if(verbosity)
    {
      cout << "P = " << p << "GeV/c" << endl;
      cout << "beta = " << beta << endl;
      cout << "Cherenkov angle = " << thetac << " rad " << thetac*180./pi << " deg" << endl;
      cout << "dN/dx = " << dndx << endl;
      cout << "npe = " << npe << endl;
      cout << "Lpath = " << ltravel << endl;
      cout << endl;
    }

  event->SetRadiator(fn, fLength, fThickness);
  event->SetAngle(fAngle);
  event->SetLambdaCutoff(fCutoff);
  event->SetSigmaT(sigmaT);
  event->SetCherenkovAngle(thetac);
  event->SetMomentum(p);
  event->SetMass(mass);
  event->SetBeta(beta);
  event->SetdNdx(dndx);
  event->SetNpe(int(npe));
 
  int nph = 0;
  for(int ipe=0; ipe<(int)npe; ipe++)
    {
      // uniformly distributed along the particle path
      double dz = rand.Uniform(0,1) * ltravel;
      double phi = rand.Uniform(-pi, pi);
      // double phi = 90.*pi/180.;

      double x = x0;
      double y = y0;
      double z = dz + z0;

      TVector3 v_dir( sin(thetac)*cos(phi), sin(thetac)*sin(phi), cos(thetac) );

      // in the (linearly) shifted coordinate with a origin of current position
      // normal vector n; zhat rotated by fAngle 
      TVector3 zhat(0, 0, 1);
      TVector3 yhat(0, 1, 0);
      zhat.RotateX(fAngle*pi/180.); 
      yhat.RotateX(fAngle*pi/180.);

      int readout; // 0:top 1: bottom 2: no detection

      //angle between photon and yprime
      double angle = v_dir.Angle(yhat);

      //distant to travel in yprime coordinate
      double lpro  = -999;
      double ydist = -999;
      double tpro  = -999;
      double tgen  = dz / (beta / c); //ps

      if( cos(angle) == 0 )
	{
	  readout = 2;
	}
      else
	{
	  if( cos(angle) < 0 )
	    {
	      ydist = fLength/2. - ( z * cos(fAngle*pi/180.) );
	      readout = 1; //bottom end
	    }
	  else
	    {
	      ydist = fLength/2. + ( z * cos(fAngle*pi/180.) );
	      readout = 0; //topend
	    }
	 
	  lpro = abs( ydist / cos(angle) );
	  tpro = fn * lpro / c; //ps
	}

      double ttot = tgen + tpro;

      if(verbosity)
	{
	  cout << "------------- npe = " << ipe << " -------------" << endl;
	  cout << "Z: " << z << " tGen: " << tgen*.1e-3 << " ns" << endl;
	  cout << "Phi: " << phi*180./pi << " Angle: " << angle*180./pi << endl;
	  cout << "Readout: " << roname[readout] << endl;
	  cout << "dY: " << ydist << " Lprop: " << lpro << " cm Time:" << tpro*1.e-3 << " ns" << endl;
	  cout << "tTot: " << ttot*1.e-3 << " ns" << endl;
	  cout << endl;
	}     

      //take too long to detect
      if( tpro > 1.e18 ) readout = 2;

      PIDPhoton ph;
      ph.x = x;
      ph.y = y;
      ph.z = z;
      ph.Phi = phi;
      ph.Angle = angle;
      ph.lPro = lpro;
      ph.tPro = tpro;
      ph.tGen = tgen;
      ph.tTot = ttot;
      ph.SensorID = readout;
      event->AddPhoton(ph);

      nph++;
    }
  return 0;

}

void ToyCherenkov::SetDetector(double n, double l, double t, double tilt_angle)
{

  // refractive index
  fn = n; 
  // length of the bar, cm
  fLength = l; 
  // thickness of the bar, cm
  fThickness = t; 
  // tilt angle between normal vector and z in deg (+: clockwise rotation)
  fAngle = tilt_angle; 
}

void ToyCherenkov::SetCutoff(double value)
{
  fCutoff = value;
}

void ToyCherenkov::SetdT(double value)
{
  sigmaT = value;
}

double ToyCherenkov::getdNdx(double p)
{
  double cer_angle = getCerAngle(p);
  double dNdx = 0;
  double integ = getIntegralQE(fCutoff);
  dNdx = 2 * pi * alpha * sin(cer_angle) * sin(cer_angle) * integ;

  return dNdx;
}

double ToyCherenkov::getIntegralQE(double cutoff)
{

  // hardcoded value copied from EIC-RICH spreadsheet for now
  // add function later
  // for 300nm lambda_cutoff (1/m)
  return 382165.2631;  
}

double ToyCherenkov::getCerAngle(double p)
{
  double angle = 0;
  if( fn*beta > 1)
    angle = acos(1./fn/beta); // in rad

  return angle;
}
