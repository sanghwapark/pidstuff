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

// bar properties
double fn         = 1.02;         // refractive index
double fLength    = 20.;          // length of the bar, cm
double fThickness = 2.;           // thickness of the bar, cm
double fAngle     = -10.;          // tilt angle between normal vector and z in deg (+:clockwise rotation)

// photon detection
double fCutoff    = 300;          // Lambda cutoff
double sigmaT     = 10.;          // ps

double beta;
double mass;
double thetac;
string particlename;

enum {ELECTRON, PION, KAON, PROTON};
string roname[3] = {"TOP", "BOTTOM", "NO SIGNAL"};

double getdNdx(double p);
double getIntegralQE(double cutoff);
double getCerAngle(double p);
void SetTree(TTree* tree);
void InitVar();

int verbosity = 1;

const int NMAX = 1000;
int    EventNumber;
int    o_pid;
double o_p;
double o_beta;
double o_thetac;
double o_dndx;
int    nPhotons;
double o_x[NMAX];
double o_y[NMAX];
double o_z[NMAX];
double o_phi[NMAX];
double o_angle[NMAX];
double o_lPro[NMAX];
double o_tPro[NMAX];
double o_tGen[NMAX];
double o_tTot[NMAX];
double o_sensorID[NMAX];

void InitVar()
{
  EventNumber = -999;
  o_pid    = -999;
  o_p      = -999;
  o_beta   = -999;
  o_thetac = -999;
  o_dndx   = -999;
  nPhotons = -999;
  for(int i=0; i<NMAX; i++)
    {
      o_x[i] = -999;
      o_y[i] = -999;
      o_z[i] = -999;
      o_phi[i] = -999;
      o_angle[i] = -999;
      o_lPro[i] = -999;
      o_tPro[i] = -999;
      o_tGen[i] = -999;
      o_tTot[i] = -999;
      o_sensorID[i] = -999;
    }
}

void SetTree(TTree* tree)
{
  tree->Branch("EventNumber", &EventNumber,"EventNumber/I");
  tree->Branch("n",           &fn,         "n/D");
  tree->Branch("length",      &fLength ,   "length/D");
  tree->Branch("thickness",   &fThickness, "thickness/D");
  tree->Branch("tilt_angle",  &fAngle,     "tilt_angle/D");
  tree->Branch("sigmaT",      &sigmaT,     "sigmaT/D");

  tree->Branch("pid",         &o_pid,        "pid/I");
  tree->Branch("p",           &o_p,          "p/D");
  tree->Branch("beta",        &o_beta,       "beta/D");
  tree->Branch("thetac",      &o_thetac,     "thetac/D");
  tree->Branch("dndx",        &o_dndx,       "dndx/D");

  tree->Branch("nPhotons", &nPhotons , "nPhotons/I");
  tree->Branch("x",        o_x,        "x[nPhotons]/D");
  tree->Branch("y",        o_y,        "y[nPhotons]/D");
  tree->Branch("z",        o_z,        "z[nPhotons]/D");
  tree->Branch("phi",      o_phi,      "phi[nPhotons]/D");
  tree->Branch("angle",    o_angle,    "angle[nPhotons]/D");
  tree->Branch("lPro",     o_lPro,     "lPro[nPhotons]/D");
  tree->Branch("tPro",     o_tPro,     "tPro[nPhotons]/D");
  tree->Branch("tGen",     o_tGen,     "tGen[nPhotons]/D");
  tree->Branch("tTot",     o_tTot,     "tTot[nPhotons]/D");
  tree->Branch("sensorID", o_sensorID, "sensorID[nPhotons]/I");
}


void dirclike(double p=1, int pid=1)
{

  cout << endl;
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

  // calculate beta and Cherenkov angle
  beta = sqrt( pow(p/mass,2) / (1. + pow(p/mass,2)) );
  thetac = (fn*beta > 1) ? acos(1./fn/beta) : 0;

  if(thetac == 0)
    {
      cout << "No Cherenkov radiation: " << particlename.c_str()
	   << " p = " << p << "GeV/c, n = " << fn << endl;
      return;
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

  cout << "P = " << p << "GeV/c" << endl;
  cout << "beta = " << beta << endl;
  cout << "Cherenkov angle = " << thetac << " rad " << thetac*180./pi << " deg" << endl;
  cout << "dN/dx = " << dndx << endl;
  cout << "npe = " << npe << endl;
  cout << "Lpath = " << ltravel << endl;
  cout << endl;

  // Output
  TFile* fout = new TFile(Form("fout_p%.2f_pid%d.root", p, pid), "RECREATE");
  TTree* T = new TTree("T","T");
  SetTree(T);
  InitVar();

  o_beta = beta;
  o_pid = pid;
  o_p   = p;
  o_thetac = thetac;
  o_dndx   = dndx;
  nPhotons = (int)npe;

  int nsig[3] = {0};
  vector<double> v_time;

  for(int ipe=0; ipe<(int)npe; ipe++)
    {
     
      v_time.clear();

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

     v_time.push_back(ttot);

     //take too long to detect
     if( tpro > 1.e18 ) readout = 2;






     nsig[readout]++;

    }//npe loop

  cout << "End of process" << endl;
  cout << "#TOP: " << nsig[0] << " #BOTTOM: " << nsig[1] << " #NOSIGNAL: " << nsig[2] << endl;
  cout << endl;

  return;
}

double getdNdx(double p)
{
  double cer_angle = getCerAngle(p);
  double dNdx = 0;
  double integ = getIntegralQE(fCutoff);
  dNdx = 2 * pi * alpha * sin(cer_angle) * sin(cer_angle) * integ;

  return dNdx;
}

double getIntegralQE(double cutoff)
{

  // hardcoded value copied from EIC-RICH spreadsheet for now
  // add function later
  // for 300nm lambda_cutoff (1/m)
  return 382165.2631;  
}

double getCerAngle(double p)
{
  double angle = 0;
  if( fn*beta > 1)
    angle = acos(1./fn/beta); // in rad

  return angle;
}
