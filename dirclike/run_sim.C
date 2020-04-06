R__LOAD_LIBRARY(libPIDToy)

void run_sim(int pid, double p, int nevt=1)
{

  ToyCherenkov* ana = new ToyCherenkov(pid);
  ana->SetVerbosity(0);
  ana->SetDetector(1.02, 20., 2., -10);
  ana->SetCutoff(300.);
  ana->SetdT(10.);

  for(int ievt=0; ievt<nevt; ievt++)
    {
      PIDEvent* anEvent = new PIDEvent();
      ana->run(p, anEvent);

      int nph = anEvent->GetNPhotons();
      cout << "I have " << nph << " photons!" << endl;

      cout << anEvent->Angle << endl;

      PIDPhoton* photons = (PIDPhoton*)anEvent->GetPhotons();
      for(int i=0; i<nph; i++)
        {
          cout << photons[i].Phi << endl;
        }

      delete[] photons;
      delete anEvent;
    }

}
