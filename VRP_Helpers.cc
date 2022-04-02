// File VRP_Helpers.cc
#include "VRP_Helpers.hh"

//IN REALTA' QUA DA CAPIRE COME USARE LO STATO
// constructor
VRP_StateManager::VRP_StateManager(const VRP_Input & pin) 
  : StateManager<VRP_Input,VRP_State>(pin, "VRP_StateManager")  {} 

// initial state builder 
void VRP_StateManager::RandomState(VRP_State& st) 
{
  
  // Insert the code that creates a random state in object st
  st.ResetState();
  unsigned id_cliente;
  unsigned id_truck;
  for(unsigned i=0; i<st.NumTruck(); i++)
  {
    do
    {
      id_cliente = Random::Uniform<unsigned>(1, in.Customers() - 1);
    } while(st.GiaServito(id_cliente) || st.VediTruckResidualCapacity(i) < in.Demand(id_cliente));
    st.Pushback(i, id_cliente);
    st.ChangeTruckResidualCapacity(i,in.Demand(id_cliente));
    st.SetServito(id_cliente);
  }

  /*debugging
  for(unsigned i=0; i < st.NumTruck(); i++)
  {
    cout<<"Camion "<<i;
    for(unsigned j=0; j < st.SizePath(i); j++)
    {
      cout<<" "<<st.VediMatriceRoute(i,j)<<" ";
    }
    cout<<endl;
  }*/

  while(!st.TuttiServiti())
  {
    do
    {
      id_truck = Random::Uniform<unsigned>(0, st.NumTruck() - 1);
      id_cliente = Random::Uniform<unsigned>(1, in.Customers() - 1);
    } while(st.GiaServito(id_cliente) || st.VediTruckResidualCapacity(id_truck) < in.Demand(id_cliente));
    //OCCHIO COSI PERO ANDRA IN LOOP INFINITO SE LA CAPACITA DEI CLIENTI SUPERA QUELLA TOTALE DEI CAMION.. dovremmo generalizzare mettendo anche la possibilita di non servire i clienti non convenienti 
    st.Pushback(id_truck, id_cliente);
    st.ChangeTruckResidualCapacity(id_truck,in.Demand(id_cliente));
    st.SetServito(id_cliente);  
  }




  for(unsigned i=0; i < st.NumTruck(); i++)
    st.Pushback(i, 0);

  //RICORDARSI DI INIZIALIZZARE QUA IL VETTORE_MOSSE Per fare la random move 
  int v = 0; 
  for(unsigned i = 0; i < st.NumTruck(); i++)
  {
    int c = st.SizePath(i) - 3;           
    v += (1 + c)/2.0 *c;                 //TRUCCO DI GAUSS A 10 ANNI (4 + 3 + 2 + 1)
    st.SetVettoreMosse(i, v);
  }


  /*
  for(unsigned t = 0; t < in.Trucks(); t++) 
  {
      for(unsigned c = 0; c < CustomersPath(t); c++) //forse CustomersPath(t) da mettere nello stato, non nell'output altrimenti devo passare l'output stesso come parametro a randomstate (se non così faccio fino a dove c'è lo 0 ma non penso abbia senso)
      {
        //da vedere se è giusto per questione probabilità bilanciata o meno
        //più questione che qui non si può fare un random su vettore dinamico dopo averlo resettato
        st(t, c) = Random::Uniform<unsigned>(0,in.Customers()-1);
      }
  }
  */

  //per fare a caso considerando a questo punto la matrice clienti truck prendo a ogni
  //giro un cliente e decido dapprima se assegnarlo, e nel caso in cui io decida di assegnarlo
  //allora valuto a che cliente e in che ordine, e per l'ordine forse parto in modo crescente a seconda di quello che è l'ultimo numero d'ordine inserito (uso struttura o metodo per questo) --> forse veramente casino per questione bilanciamento probabilità

  //update dati ridondanti se gli mettiamo
  st.UpdateRedundantStateData();
} 


bool VRP_StateManager::CheckConsistency(const VRP_State& st) const
{
  // Insert the code that checks if state in object st is consistent
  // (for debugging purposes)
	
  return true;

  //SEMPLICEMENTE VALUTO SE CLIENTI SERVITI SONO QUELLI CHE MI CALCOLA IL MIO METODO DI CLASSE, MAGARI ANCHE ORDINE (?), E DA CHI SONO SERVITI
  //QUINDI CONFRONTO TRA METODO E FACCIO DEI CICLI SALVANDO I DATI CHE POI VENGONO CONFRONTATI
/*
  if(c'è un errore)
    return false;

  //altrimenti tutto ok
  return true;
  */
}

//DA VEDERE COSA METTERE COME COMPONENTI DI COSTO, OGNI PRINT E' COME IL COMPUTE CON NEL CICLO UNA STAMPA AL POSTO DI UN INCREMENTO/DECREMENTO CONTATORE

int VRP_DistanceCost::ComputeCost(const VRP_State& st) const
{
  unsigned cost = 0;
  // Insert the code that computes the cost for component 1 of state st
  for(unsigned i=0; i < st.NumTruck(); i++)
    for(unsigned j=0; j < st.SizePath(i) - 1; j++)      //messo -1 per evitare una somma finale tra 0 e niente
      cost += in.DistanzaCustomer(st.VediMatriceRoute(i,j), st.VediMatriceRoute(i,j+1));

  return cost;
}

          
void VRP_DistanceCost::PrintViolations(const VRP_State& st, ostream& os) const
{
  // Insert the code that prints the violations of component 1 
  // of state st
		
}


/*****************************************************************************
 * Output Manager Methods
 *****************************************************************************/

//FACILE, SOLO DA CAPIRE COSA USARE COME OUTPUT E COSA COME STATE
void VRP_OutputManager::InputState(VRP_State& st, const VRP_Output& out) const 
{
  // Insert the code that "translates" an output object to a state object
  
  unsigned i, j;
  for (i = 0; i < out.SelectedPaths(); i++)
    for (j = 0; j < out.CustomersPath(i); j++)    
      st.ChangeMatriceRoute(i,j,out(i,j));
  //aggiorno dati ridondandi stato
  st.UpdateRedundantStateData(); 
}

void VRP_OutputManager::OutputState(const VRP_State& st, VRP_Output& out) const 
{
  // Insert the code that "translates" a state object to an output object
  unsigned i, j;
  for (i = 0; i < st.NumTruck(); i++)
  {
    out.ResetPath(i);
    for (j = 0; j < st.SizePath(i); j++)  
      out.ChangeInPaths(i,st.VediMatriceRoute(i,j));
  }
}

/*****************************************************************************
 * VRP_Move Neighborhood Explorer Methods
 *****************************************************************************/


//SEMPRE CHE ATTRIBUTI MOSSA SIANO CORRETTI, ALTRIMENTI DA CAMBIARE

void VRP_MoveNeighborhoodExplorer::RandomMove(const VRP_State& st, VRP_Move1& mv) const
{
  // insert the code that writes a random move on mv in state st
  //da vedere se scegliere il random di qualeroute in base a quanti clienti ha ogni route (prediligere le route con piu clienti perche hanno piu mosse)
  unsigned q = Random::Uniform<unsigned>(0, st.VediVettoreMosse(st.NumTruck() - 1) - 1 );
  unsigned i = 0;
  unsigned temp;

  while(q >= st.VediVettoreMosse(i))  
    i++;

  mv.qualeroute = i;
	mv.cust1 = Random::Uniform<unsigned>(1, st.SizePath(i) - 2); // -2 per i depositi 
  mv.cust2 = Random::Uniform<unsigned>(1, st.SizePath(i) - 3); //-3 per i 2 depositi piu il cliente sopra
  if(mv.cust2 >= mv.cust1)
    mv.cust2++;
  else
  {
    temp = mv.cust2;
    mv.cust2 = mv.cust1;
    mv.cust1 = temp;
  }

} 

void VRP_MoveNeighborhoodExplorer::MakeMove(VRP_State& st, const VRP_Move1& mv) const
{
  // Insert the code that modify the state st based on the application of move mv
  unsigned temp1, temp2;
  temp1 = st.VediMatriceRoute(mv.qualeroute, mv.cust1);
  temp2 = st.VediMatriceRoute(mv.qualeroute, mv.cust2);
  st.ChangeMatriceRoute(mv.qualeroute, mv.cust1, temp2);
  st.ChangeMatriceRoute(mv.qualeroute, mv.cust2, temp1);
	
}  


bool VRP_MoveNeighborhoodExplorer::FeasibleMove(const VRP_State& st, const VRP_Move1& mv) const
{
  // Insert the code that check is move mv is legal in state st 
  // (return true if legal, false otherwise)


  //questo metodo va implementato solo quando facciamo la mossa che scambia due clienti tra camion diversi per controllare se sforo la capacità
  //in questo caso siccome scambiamo nello stesso percorso la capacità non varia di una certo path
  return true;
} 


void VRP_MoveNeighborhoodExplorer::FirstMove(const VRP_State& st, VRP_Move1& mv) const
{
  // Insert the code the generate the first move in the neighborhood and store it in mv
  mv.cust1 = 1;  
  mv.cust2 = 2;
  mv.qualeroute = 0; //inizializzo al primo truck
  while(st.SizePath(mv.qualeroute) < 4)  // messo 4 perchè non mi va bene avere solo 0 - cliente - 0 ma servono almeno 2 clienti 
    mv.qualeroute++;
  
}


bool VRP_MoveNeighborhoodExplorer::NextMove(const VRP_State& st, VRP_Move1& mv) const
{
  // Insert the code that generate the move that follows mv in the neighborhood, and writes
  // it back in mv. Return false if mv is already the last move. 
  if(mv.cust2 < st.SizePath(mv.qualeroute) - 2)
    mv.cust2++;
  else
  {
    mv.cust1++;
    mv.cust2 = mv.cust1 + 1;  //per evitare di avere lo swap tra cust1 e cust2 uguali
  }
  if(mv.cust1 > st.SizePath(mv.qualeroute) - 3)
  {
    mv.qualeroute++;
    while(st.SizePath(mv.qualeroute) < 4)  // messo 4 perchè non mi va bene avere solo 0 - cliente - 0 ma servono almeno 2 clienti 
      mv.qualeroute++;
    mv.cust1 = 1;
    mv.cust2 = 2;
  }
  if(mv.qualeroute > st.NumTruck())
    return false;  //ho finito le mosse 

  return true;
}

int VRP_MoveDeltaDistanceCost::ComputeDeltaCost(const VRP_State& st, const VRP_Move1& mv) const
{
  // Insert the code that computes the delta cost of component 1 for move mv in state st
  int cost = 0;
  cost -= in.DistanzaCustomer(st.VediMatriceRoute(mv.qualeroute,mv.cust1 - 1), st.VediMatriceRoute(mv.qualeroute,mv.cust1));
	cost -= in.DistanzaCustomer(st.VediMatriceRoute(mv.qualeroute,mv.cust1), st.VediMatriceRoute(mv.qualeroute,mv.cust1 + 1));
	cost -= in.DistanzaCustomer(st.VediMatriceRoute(mv.qualeroute,mv.cust2 - 1), st.VediMatriceRoute(mv.qualeroute,mv.cust2));
	cost -= in.DistanzaCustomer(st.VediMatriceRoute(mv.qualeroute,mv.cust2), st.VediMatriceRoute(mv.qualeroute,mv.cust2 + 1));

  cost += in.DistanzaCustomer(st.VediMatriceRoute(mv.qualeroute,mv.cust1 - 1), st.VediMatriceRoute(mv.qualeroute,mv.cust2));
	cost += in.DistanzaCustomer(st.VediMatriceRoute(mv.qualeroute,mv.cust2), st.VediMatriceRoute(mv.qualeroute,mv.cust1 + 1));
	cost += in.DistanzaCustomer(st.VediMatriceRoute(mv.qualeroute,mv.cust2 - 1), st.VediMatriceRoute(mv.qualeroute,mv.cust1));
	cost += in.DistanzaCustomer(st.VediMatriceRoute(mv.qualeroute,mv.cust1), st.VediMatriceRoute(mv.qualeroute,mv.cust2 + 1));

	//se delta è da minimizzare bisogna sopra -= e sotto +=
 //da vedere se il delta è positivo o negativo (in caso invertire i meno con i piu )
  return cost;
}






