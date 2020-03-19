#include "Sim.C"

void run_sim(int pid)
{

  Sim ana(pid);

  PIDEvent* anEvent = new PIDEvent();

  ana.SetDetector(1.02, 20., 2., -10);
  ana.SetCutoff(300.);
  ana.SetdT(10.);
  
  ana.run(2, anEvent);

}
