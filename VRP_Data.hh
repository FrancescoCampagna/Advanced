// File VRP_Data.hh
#ifndef VRP_DATA_HH
#define VRP_DATA_HH

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

using namespace std;

class VRP_Input 
{
    friend ostream& operator<<(ostream& os, const VRP_Input& in);
    //eventually others

  public:
    VRP_Input(string file_name);
    // Insert your getters
    //eventually time capacity and individual service time

    unsigned Customers() const{return customers; }
    unsigned Trucks() const {return trucks; }
    unsigned Capacity() const {return capacity; }

    void setNumbOfTruck(unsigned i) {trucks = i; } //per Greedy, o qui o stato
    
    pair<int,int> getCustomerPosition (unsigned i) const {return customer_map[i];}
    int NodeX (unsigned i) const {return customer_map[i].first; }
    int NodeY (unsigned i) const {return customer_map[i].second; }
    double DistanzaCustomer(unsigned i, unsigned j) const {return customer_distances[i][j]; } //VEDI
    unsigned Round(double n); //--> VEDI

    unsigned Demand (unsigned i) const {return demand[i]; }
    double Prize (unsigned i) const {return prize[i]; }

    //COSTANTI IPOTIZZATE probabilmente da mettere qua
    const double FUEL_COST_LITER = 1.39;         //euro
    const double AVG_TRUCK_CONS_LITER = 13;      //km con un litro
    const double GAIN_THRESHOLD = 0.0;           //da mettere?

  protected:
    // Insert your data members
    unsigned customers, trucks, capacity;    //customers and trucks number, and vehicle capacity                         //capacità di ogni camion
    vector <pair<int,int> > customer_map;    //x and y coordinates of each customer
    vector <unsigned> demand;                //demand from each customer served, 
    vector <double> prize;                   //profits from each customer served
    vector<vector<double> > customer_distances; //VEDI
};

class VRP_Output 
{
  friend ostream& operator<<(ostream& os, const VRP_Output& out);
  friend istream& operator>>(istream& is, VRP_Output& out);
public:
  VRP_Output(const VRP_Input& in);
  VRP_Output& operator=(const VRP_Output& out);
  // Insert your getters
  
  unsigned operator()(unsigned i, unsigned j) const { return paths[i][j]; }
  unsigned& operator()(unsigned i, unsigned j) { return paths[i][j]; }

  //QUESTE VANNO MESSE SE VETTORE DI LISTE FRASTAGLIATE
  unsigned CustomersPath(unsigned i) const { return paths[i].size(); }
  vector<unsigned> SelectedPath(unsigned i) const { return paths[i]; }
  unsigned SelectedPaths() const { return paths.size(); }
  void AssignCustomerToTruck(unsigned c, unsigned t) { paths[t].push_back(c); }
  void RemoveCustomerToTruck(unsigned c, unsigned t);
  void ResetPath(unsigned i) { paths[i].clear(); } 


  void ChangeInPaths(unsigned i, unsigned new_value) { paths[i].push_back(new_value); }
  //altrimenti il reset se uso unica matrice lo devo mettere solo nello stato e inizializzare a 0 o comunque resettare le mie strutture

  //da vedere se tenere
  unsigned RouteCapacity(int i) const;
  unsigned RouteCost(int i) const;

  bool ValidSolution() const; //da togliere nel caso generale

protected:
  const VRP_Input& in;

  vector<int> feasible; //da togliere quando si generalizza - si considerano prezzi (-1 se non servito)

  // Insert your data members
  //here the enumeration start from 1 for the customer, 0 is the depot
  vector<vector<unsigned> > paths;     //(truck x customer) for each truck memorize the index of the customer for the corrsepondent order it is served by the truck, starting with all 0 (dim_max = n_customers)												
};




#endif
