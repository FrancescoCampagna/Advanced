// File VRP_Basics.cc
#include "VRP_Basics.hh"

VRP_State::VRP_State(const VRP_Input &my_in) 
  : in(my_in), 
    customer_location(my_in.Customers(), make_pair(0,0)), 
    vettore_mosse(my_in.Trucks()),
    matrice_route(my_in.Trucks()), 
    truckresidualcapacity(my_in.Trucks()), 
    giaservito(my_in.Customers())
  //vedi foglietto frank per dim vettore_mosse
{} //sempre questione array dinamico o no




VRP_State& VRP_State::operator=(const VRP_State& st)
{
  // Insert the code that copies all data structures of the
  // state object from the ones of the parameter out
  // (excluding the reference to the input object, that is constant)
  customer_location = st.customer_location;
  vettore_mosse = st.vettore_mosse;
  matrice_route = st.matrice_route;
  return *this;
}

bool operator==(const VRP_State& st1, const VRP_State& st2)
{
  // Insert the code that compares two states (this is used by the tester)
  return st1.customer_location == st2.customer_location 
      && st1.vettore_mosse == st2.vettore_mosse
      && st1.matrice_route == st2.matrice_route; 
  
  //basta?
}


ostream& operator<<(ostream& os, const VRP_State& st)
{
  // Insert the code that writes the state object (for debugging)
  //ricorda di allineare la stampa intestazione con valori stato
  unsigned i;
  os << "VRP_State: " << endl;
  os << "TruckID" << setw(3) << "Order" << endl;
  for (i = 0; i < st.customer_location.size(); i++)
      os << setw(3) << st.customer_location[i].first << setw(3) << st.customer_location[i].second << endl; //metto tre spazi
  os << endl;

  os << "Matrice route (ridondante): " << endl;
  for(unsigned i=0; i < st.NumTruck(); i++)
  {
    for(unsigned j=0; j < st.SizePath(i); j++)
      os << st.VediMatriceRoute(i, j) << setw(3);
    
    os << endl;
  }

  return os;
}


bool VRP_State::TuttiServiti()
{
  for(unsigned i = 1; i < giaservito.size(); i++)    //parto da 1 altrimenti mi conta il deposito che sara sempre false
    if(!giaservito[i])
      return false;
  return true;
}



//##########################################################################################################################################
//##########################################################################################################################################

void VRP_State::ResetState()
{
  for(unsigned i=0; i < in.Trucks(); i++)
  {
    
    truckresidualcapacity[i] = in.Capacity();
    matrice_route[i].clear();
    matrice_route[i].push_back(0);
    vettore_mosse[i] = 0;
  }
  for(unsigned i=0; i < in.Customers(); i++)
  {
    giaservito[i] = false;
    customer_location[i].first = -1;    //cliente non servito: metto il camion -1
    customer_location[i].second = 1;     // posizione 1 perchè prima c'è il deposito 
  }

}

void VRP_State::UpdateRedundantStateData() 
{
  	
}
//##########################################################################################################################################
//##########################################################################################################################################

VRP_Move1::VRP_Move1()
{
  // Insert the code that initialize the move
  //numero d'ordine servizio complessivo non avrebbe senso
  cust1 = 1;
  cust2 = 1;
  qualeroute = 0;
}

bool operator==(const VRP_Move1& mv1, const VRP_Move1& mv2)
{
  // Insert the code that checks if two moves are identical
  return mv1.cust1 == mv2.cust1
    && mv1.cust2 == mv2.cust2
    && mv1.qualeroute == mv2.qualeroute;
}

bool operator!=(const VRP_Move1& mv1, const VRP_Move1& mv2)
{
  // Insert the code that checks if two moves are different
  return mv1.cust1 != mv2.cust1
    || mv1.cust2 != mv2.cust2
    || mv1.qualeroute != mv2.qualeroute;
}

bool operator<(const VRP_Move1& mv1, const VRP_Move1& mv2)
{
  // Insert the code that checks if one move precedes another one
  // (in any selected order)
  return (mv1.cust1 < mv2.cust1)
    || (mv1.cust1 == mv2.cust1 && mv1.cust2 < mv2.cust2)
    || (mv1.cust1 == mv2.cust1 && mv1.cust2 == mv2.cust2 && mv1.qualeroute < mv2.qualeroute);
}

istream& operator>>(istream& is, VRP_Move1& mv)
{
  // Insert the code that read a move
  char ch;
  return is >> mv.qualeroute >> ch >> mv.cust1 >> ch >> ch >> ch >> mv.cust2;
}

ostream& operator<<(ostream& os, const VRP_Move1& mv)
{
  // Insert the code that writes a move
  return os << mv.qualeroute + 1<< ") " << mv.cust1 + 1 << " <-> " << mv.cust2 + 1; //eventualmente endl
}
//3) 1 <-> 2

