// File VRP_Basics.hh
#ifndef VRP_BASICS_HH
#define VRP_BASICS_HH

#include "VRP_Data.hh"
#include <iomanip>

//classe di stato simile a quella di output ma con diverse ridondanze per accelerare e rendere efficienti i delta costo che per la ricerca locale sono molto complessi e devono essere molto efficienti
class VRP_State
{  
  friend ostream& operator<<(ostream& os, const VRP_State& st);
  friend bool operator==(const VRP_State& st1, const VRP_State& st2);
public:
  VRP_State(const VRP_Input &in);
  VRP_State& operator=(const VRP_State& s);
  unsigned NumTruck() const { return in.Trucks(); }
  unsigned SizePath(unsigned i) const { return matrice_route[i].size(); }
  unsigned VediMatriceRoute(unsigned i, unsigned j) const { return matrice_route[i][j]; }
  void ChangeMatriceRoute(unsigned i, unsigned j, unsigned new_value) { matrice_route[i][j] = new_value; }
  unsigned VediVettoreMosse(unsigned i) const { return vettore_mosse[i]; }
  void SetVettoreMosse(unsigned i, unsigned v) { vettore_mosse[i] = v; }
  void Pushback(unsigned i, unsigned valore) { matrice_route[i].push_back(valore); } //quindi nel costruttore mettiamo solo uno 0 e poi l altro lo aggiungiamo quando facciamo il primo stato 
  unsigned VediTruckResidualCapacity(unsigned i) const { return truckresidualcapacity[i]; }
  void ChangeTruckResidualCapacity (unsigned i, unsigned v) { truckresidualcapacity[i] -= v; } 
  bool GiaServito(unsigned i) const { return giaservito[i]; }
  void SetServito(unsigned i) { giaservito[i] = true; }
  bool TuttiServiti();
  void ResetState();
  void UpdateRedundantStateData();

protected:
  const VRP_Input & in;  
  vector<unsigned> vettore_mosse; //da vedere dove inizializzarla [vedi appunti frenk di quantum computing]
  vector<pair<unsigned,unsigned> > customer_location; //righe = clienti con info truck che serve e ordine (vedi excel)
  vector<vector<unsigned> > matrice_route; //da vedere come inizializzarla
  vector<unsigned> truckresidualcapacity;
  vector<bool> giaservito;
};

class VRP_Move1
{
  friend bool operator==(const VRP_Move1& m1, const VRP_Move1& m2);
  friend bool operator!=(const VRP_Move1& m1, const VRP_Move1& m2);
  friend bool operator<(const VRP_Move1& m1, const VRP_Move1& m2);
  friend ostream& operator<<(ostream& os, const VRP_Move1& c);
  friend istream& operator>>(istream& is, VRP_Move1& c);
 public:
  VRP_Move1();
  unsigned cust1, cust2; // inverte i clienti i e j
  unsigned qualeroute; //rappresenta il numero del truck nel quale sto facendo lo scambio
};
#endif

