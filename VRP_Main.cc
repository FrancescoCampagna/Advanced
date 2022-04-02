#include "VRP_Helpers.hh"

using namespace EasyLocal::Debug;

int main(int argc, const char* argv[])
{
  ParameterBox main_parameters("main", "Main Program options");

  // The set of arguments added are the following:
  
  Parameter<string> instance("instance", "Input instance", main_parameters); 
  Parameter<int> seed("seed", "Random seed", main_parameters);
  Parameter<string> method("method", "Solution method (empty for tester)", main_parameters);   
  Parameter<string> init_state("init_state", "Initial state (to be read from file)", main_parameters);
  Parameter<string> output_file("output_file", "Write the output to a file (filename required)", main_parameters);
 
  // 3rd parameter: false = do not check unregistered parameters
  // 4th parameter: true = silent
  CommandLineParameters::Parse(argc, argv, false, true);  

  if (!instance.IsSet())
  {
    cout << "Error: --main::instance filename option must always be set" << endl;
    return 1;
  }
  VRP_Input in(instance); //EVENTUALI PESI SE NON LI DO DIRETTAMENTE E PRENDO DA ISTANZA METTO PESI QUI ES (), E NELLA CLASSE DI INPUT NON ASSEGNO VALORI MA DO SOLO NOMI, SOTTO INVECE DO COME ARGOMENTO COST COMPONENT in(instance, {p1, p2})
  //FORSE output object ma non dice, su TT però si --> VRP_Output out(in);

  if (seed.IsSet())
    Random::SetSeed(seed);
  
  //A SECONDA DI COME LE CHIAMO E QUANTE NE METTO CAMBIA
  // cost components: second parameter is the cost, third is the type (true -> hard, false -> soft)
  VRP_DistanceCost cc1(in, 1, true); //(es pesi scritti nelle costanti) al posto di uno metto in.COSTANTE
  //VRP_CostComponent2 cc2(in, 1, true);     
 
  VRP_MoveDeltaDistanceCost dcc1(in, cc1);
  //VRP_MoveDeltaCostComponent2 dcc2(in, cc2);

  // helpers
  VRP_StateManager VRP_sm(in);
  VRP_MoveNeighborhoodExplorer VRP_nhe(in, VRP_sm);

  VRP_OutputManager VRP_om(in);
  
  // All cost components must be added to the state manager
  VRP_sm.AddCostComponent(cc1);
  //VRP_sm.AddCostComponent(cc2);
  
  // All delta cost components must be added to the neighborhood explorer
  VRP_nhe.AddDeltaCostComponent(dcc1);
  //VRP_nhe.AddDeltaCostComponent(dcc2);
  
  // runners
  HillClimbing<VRP_Input, VRP_State, VRP_Move1> VRP_hc(in, VRP_sm, VRP_nhe, "VRP_MoveHillClimbing");
  SteepestDescent<VRP_Input, VRP_State, VRP_Move1> VRP_sd(in, VRP_sm, VRP_nhe, "VRP_MoveSteepestDescent");
  SimulatedAnnealing<VRP_Input, VRP_State, VRP_Move1> VRP_sa(in, VRP_sm, VRP_nhe, "VRP_MoveSimulatedAnnealing");

  // tester
  Tester<VRP_Input, VRP_Output, VRP_State> tester(in,VRP_sm,VRP_om);
  MoveTester<VRP_Input, VRP_Output, VRP_State, VRP_Move1> swap_move_test(in,VRP_sm,VRP_om,VRP_nhe, "VRP_Move move", tester); 

  SimpleLocalSearch<VRP_Input, VRP_Output, VRP_State> VRP_solver(in, VRP_sm, VRP_om, "VRP solver");
  if (!CommandLineParameters::Parse(argc, argv, true, false))
    return 1;

  if (!method.IsSet())
  { // If no search method is set -> enter in the tester
    if (init_state.IsSet())
    tester.RunMainMenu(init_state);
    else
    tester.RunMainMenu();
  }
  else
  {
    if (method == string("SA"))
    {
      VRP_solver.SetRunner(VRP_sa);
    }
    else if (method == string("HC"))
    {
      VRP_solver.SetRunner(VRP_hc);
    }
    else // if (method.GetValue() == string("SD"))
    {
      VRP_solver.SetRunner(VRP_sd);
    }

    /*potrei aggiungere anche questo
    else
    {
      cerr << "unrecognized method " << string(method) << endl;
      exit(1);
    }*/

    auto result = VRP_solver.Solve();
    // result is a tuple: 0: solutio, 1: number of violations, 2: total cost, 3: computing time
    VRP_Output out = result.output;
    if (output_file.IsSet())
    { // write the output on the file passed in the command line
      ofstream os(static_cast<string>(output_file).c_str());
      os << out << endl;
      os << "Cost: " << result.cost.total << endl;
      os << "Time: " << result.running_time << "s " << endl;
      os.close();
    }
    else
    { // write the solution in the standard output
      cout << out << endl;
      cout << "Cost: " << result.cost.total << endl;
      cout << "Time: " << result.running_time << "s " << endl;					
    }
  }
  return 0;
}
