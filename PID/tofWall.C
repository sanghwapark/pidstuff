#include "PIDConstant.h"
#include "tofWall.h"

using namespace std;

tofWall::tofWall(double rmin, double rmax, double z, double sT)
{

  zposition = z;

  // To avoid eta->infinity at zero deg, need to replace 0.1 with inner radius limit by beam pipe
  if( rmin == 0)
    radiusIn = 0.1;
  else
    radiusIn = rmin;    

  radiusOut = rmax;

  double eta1 = -log(tan( atan2(radiusOut,z)/2. ));
  double eta2 = -log(tan( atan2(radiusIn,z)/2. ));
  etaLow  = (eta1 < eta2) ? eta1 : eta2;
  etaHigh = (eta1 < eta2) ? eta2 : eta1;
  sigmaT  = sT;

  char nameit[500];
  sprintf(nameit,"TOF wall R=(%d,%d) Z=%d dT=%d",int(rmin), int(rmax), int(zposition), int(sigmaT));
  myName = nameit;
}

double tofWall::numSigma(double eta, double p, PID::type PID)
{
  if (valid(eta,p))
    {
      double theta = 2.0*atan( exp(-eta) );
      double L = zposition/cos(theta);
      if (PID == pi_k)
	{
	  return (tof(L,p,mKaon)-tof(L,p,mPion))/sigmaT;
	}
      if (PID == k_p)
	{
	  return (tof(L,p,mProton)-tof(L,p,mKaon))/sigmaT;
	}
      cout << "tofWall.C:  Unrecognized PID type requested." <<endl;
      return 0.0;
    }
  else
    {
      cout << "tofWall.C:  Invalid (eta,p) for this detector." <<endl;
      return 0.0;
    }
}

double tofWall::maxP(double eta, double numSigma, PID::type PID)
{
  //
  //  Justification of formula:
  //  Tof1 = L/c *1/sqrt( p^2 / (m1^2 + p^2) ) = L/c * 1/p * sqrt(m1^2 + p^2) 
  //  Tof2 = L/c *1/sqrt( p^2 / (m2^2 + p^2) ) = L/c * 1/p * sqrt(m2^2 + p^2) 
  //  Require:  numSigma*sigmaT = Tof1 - Tof2
  //  Therefore:  (c/L) * numSigma *sigmaT = 1/p * [ sqrt(m1^2 + p^2) - sqrt(m1^2 + p^2) ]
  //  Let:
  //     a = c/L * numSigma * sigmaT
  //     b = m1
  //     c = m2
  //  Wolfram Alpha:
  //  https://www.wolframalpha.com/input/?i=solve+1%2Fx*%28sqrt%28x%5E2%2Bb%5E2%29+-+sqrt%28x%5E2%2Bc%5E2%29%29%3Da+for+x

  if (valid(eta,1.0))
    {
      double theta = 2.0*atan( exp(-eta) );
      double L = zposition/cos(theta);

      double a = c/L * numSigma * sigmaT;
      double b=mKaon;
      double c=mPion;
      if (PID == k_p)
	{
	  b=mProton;
	  c=mKaon;
	}
      
      double num1 = a*a*b*b;
      double num2 = 2*sqrt(a*a*(a*a*b*b*c*c + b*b*b*b -2*b*b*c*c + c*c*c*c));
      double num3 = a*a*c*c;
      double denom = a*a*a*a - 4*a*a;

      return ( sqrt( (num1-num2+num3)/denom ) );

    }
  else
    {
      cout << "tofWall.C:  Invalid (eta) for this detector." <<endl;
      return 0.0;
    }

}

double tofWall::tof(double L, double p, double m)
{
  double p2_m2 = p*p/(m*m);
  double beta  = sqrt(p2_m2/(1+p2_m2));
  return (L/(c*beta));
}

void tofWall::description()
{
  //  Here one should describe what this detector is and what assumptions are 
  //  made in calculating the performance specifications.

  cout << "My name is \"" << myName << "\" and I am described as follows:" <<endl;
  cout << "    I am a Time-of-Flight wall." <<endl;
  cout << "    I extend from eta =  " << etaLow << " until eta = " << etaHigh << " ." <<endl;
  cout << "    I am located at R = (" << radiusIn << ", " << radiusOut << ") Z= " << zposition << " cm." <<endl;
  cout << "    I have a time resolution of " << sigmaT << " picoseconds." <<endl;
  cout << "    My calculations have assumed perfect momentum resolution and pointing." <<endl;
  cout << "    My calculations have assumed purely Gaussian time response." <<endl;
  cout << endl;

}
