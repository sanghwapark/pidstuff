#include "PIDEvent.C"

void simple_test()
{

  PIDEvent* ev = new PIDEvent();

  ev->SetRadiator(1., 1., 1);
  ev->SetAngle(15.);
  ev->SetLambdaCutoff(300);
  ev->SetSigmaT(10.);

  for(int i=0; i<3; i++)
    {
      PIDPhoton ph;
      ph.x = i;
      ph.y = i;
      ph.z = i;
      ph.Phi = i;
      ph.Angle = i;
      ph.lPro = i;
      ph.tPro = i;
      ph.tGen = i;
      ph.tTot = i;
      ph.SensorID = i;
      ev->AddPhoton(ph);
    }

  int nph = ev->GetNPhotons();
  cout << "I have " << nph << " photons!" << endl;  

  PIDPhoton* photons = (PIDPhoton*)ev->GetPhotons();
  if( photons == NULL )
    cout << "empty!" << endl;

  for(int i=0; i<3; i++)
    {
      cout << "you don't do anything" << endl;
      cout << photons[i].x << endl;
    }

}
