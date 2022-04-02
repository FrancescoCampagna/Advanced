// File VRP_Data.cc
#include "VRP_Data.hh"
#include <fstream>
//#include <cassert>

VRP_Input::VRP_Input(string file_name)
{
  // Insert the code that reads the input from the file and stored it into the data structures of the object 
  ifstream is(file_name);

  //assert(!is.fail());
  if(is.fail())
      throw runtime_error("Cannot open instance file");

  int x, y, customer_demand, time, bin_time;
  double customer_prize;
  string bin;
   
  //ignore file intestation "Name chriX" and "MAXVEHICLES in the following line"
  is >> bin >> bin >> bin;

  //read the number of trucks available, ignore "MAXCAPACITY", read the max capacity of each vehicle and ignore the line about the time capacity (not used for this project)
  is >> trucks >> bin >> capacity >> bin >> bin_time;

  //ignore "DEPOT" and read the coordinates of the depot
  is >> bin >> x >> y;

  //ignore "CUSTOMERS" and read the number of customers
  is >> bin >> customers;     

  //add the depot to the different plants considered
  customers += 1;
  
  //ignore "CUSTOMERDATA"
  is >> bin;
  
  customer_map.resize(customers);
  demand.resize(customers);
  prize.resize(customers);
  customer_distances.resize(customers);
  for(unsigned i = 0; i < customers; i++)
    customer_distances[i].resize(customers);

  //set the data for the depot
  customer_map[0] = make_pair(x,y); 
  demand[0] = 0;
  prize[0] = 0;
  
  for(unsigned i = 1; i < customers; i++)
  {
    //read data for each customer and assign them to the different data structures
    is >> x >> y >> customer_demand >> time >> customer_prize;
    customer_map[i] = make_pair(x,y);
    demand[i] = customer_demand;
    prize[i] = customer_prize;
  }


  //non ha senso ridurre il calcolo solo a metà matrice anche se l'altra è speculare
  for(unsigned i = 0; i < customers; i++)
    for(unsigned j = 0; j < customers; j++)
      customer_distances[i][j] = sqrt(pow(customer_map[i].first - customer_map[j].first, 2) + pow(customer_map[i].second - customer_map[j].second, 2)); //vedi se mettere round alla radice
}

ostream& operator<<(ostream& os, const VRP_Input& in)
{
  // Insert the code that write the input object (needed for debugging purposes)
  os << "DATA:" << endl;
  os << in.customers << in.trucks << in.capacity << endl;
  //customers data
  for(unsigned i = 0; i < in.customers; i++)
    os << i << ") " << in.NodeX(i) << ", " << in.NodeY(i) << ", " << in.Demand(i) << ", " << in.Prize(i) << endl;

  for(unsigned i = 0; i < in.customers; i++)
  {
    os << endl;
    for(unsigned j = 0; j < in.customers; j++)
      os << in.customer_distances[i][j] << " ";
  }
  os << endl;
  return os;
}

unsigned VRP_Input::Round(double n)
{
  if((n - static_cast<unsigned>(n)) >= 0.5)
    return ceil(n);
  return floor(n);
}

//DA FARE
/*
class VRP_Output 
{
  friend ostream& operator<<(ostream& os, const VRP_Output& out);
  friend istream& operator>>(istream& is, VRP_Output& out);
public:
  VRP_Output(const VRP_Input& in);
  VRP_Output& operator=(const VRP_Output& out);
  // Insert your getters
  //eventuali inserimenti, rimozioni e reset per BDS, in TT invece update e reset state nello stato

  unsigned operator()(unsigned i, unsigned j) const { return paths[i][j]; }
  unsigned& operator()(unsigned i, unsigned j) { return paths[i][j]; }

protected:
  const VRP_Input& in;
  // Insert your data members
  //here the enumeration start from 1 for the customer, 0 is the depot
  vector<vector<unsigned> > paths;     //(truck x customer) array containing the paths covered by each truck --> value = order, 0 if a customer is not served by the correspondent truck
};
*/

VRP_Output::VRP_Output(const VRP_Input& my_in)
  : in(my_in), feasible(my_in.Customers(), -1), paths(my_in.Trucks()) //da modificare
  //eventualmente da buttare qua primo 0
{}
//da vedere cosa mettere per clienti a seconda di come considero il depot
//0 come valore vuol dire che quel cliente non è servito da quel truck

VRP_Output& VRP_Output::operator=(const VRP_Output& out)	
{
  // Insert the code that copies all data structures of the
  // output object from the ones of the parameter out
  // (excluding the reference to the input object, that is constant)
  paths = out.paths;
  feasible = out.feasible; //da togliere nel generale
  return *this;
}

ostream& operator<<(ostream& os, const VRP_Output& out)
{ 
 // Insert the code that writes the output object
 // così se uso matrice con per ciascun truck l'indice del cliente che serve considerando anche l'ordine di servizio
 //--> forse con questa struttura ho delle liste frastagliate? quindi uso push_back e clear (?)
  unsigned i, j;

  for (i = 0; i < out.paths.size(); i++)
  {
    os << "Truck " << i+1 << ": ";
    for (j = 0; j < out.paths[i].size(); j++)
      os << " " << out.paths[i][j];
    
    os << endl;
  }

  return os;
}

istream& operator>>(istream& is, VRP_Output& out)
{
  // Insert the code that reads the output object
  unsigned i, num;
  string bin_truck;
  char ch;

  out.paths.clear();   //pulisco probabilmente l'oggetto di output
  out.feasible.clear(); //vedi

  out.paths.resize(out.in.Trucks());
  out.feasible.resize(out.in.Customers());
    
  for(i = 0; i < out.paths.size(); i++)
  {
    is >> bin_truck >> ch >> num; //ipotizzo qui che truck siano in ordine nell'output, cioè pari a i+1 e non memorizzo il depot
    out.paths[i].push_back(num);  
    is >> num; //primo cliente
    do
    {
      out.paths[i].push_back(num); //forse push_back
      is >> num;
    } while(num != 0);
  }

  return is; 
}
  