// File VRP_Helpers.hh
#ifndef VRP_HELPERS_HH
#define VRP_HELPERS_HH

#include "VRP_Basics.hh"
#include <easylocal.hh>

using namespace EasyLocal::Core;

//VERSIONE PER LISTE FRASTAGLIATE o VERSIONE PER MATRICE
/***************************************************************************
 * State Manager 
 ***************************************************************************/

class VRP_StateManager : public StateManager<VRP_Input,VRP_State> 
{
public:
  VRP_StateManager(const VRP_Input & pin); //inizializzata nel file.cc ma potevo anche qui
  void RandomState(VRP_State&);   
  bool CheckConsistency(const VRP_State& st) const;
protected:
  //void ResetState(VRP_State& st); probabilmente solo questa oppure nemmeno questa vedendo dal BDS, basta metterla nella classe stato
}; 

// IN TEORIA MANCA SOLO METTERE I NOMI DELLE COMPONENTI DI COSTO E DEI RISPETTIVI DELTA DA QUESTO FILE

class VRP_DistanceCost : public CostComponent<VRP_Input,VRP_State> 
{
public:
  VRP_DistanceCost(const VRP_Input & in, int w, bool hard) :    CostComponent<VRP_Input,VRP_State>(in,w,hard,"VRP_DistanceCost") 
  {}
  int ComputeCost(const VRP_State& st) const;
  void PrintViolations(const VRP_State& st, ostream& os = cout) const;
};

/*
class  VRP_CostComponent2: public CostComponent<VRP_Input,VRP_State> 
{
public:
  VRP_CostComponent2(const VRP_Input & in, int w, bool hard) : CostComponent<VRP_Input,VRP_State>(in,w,hard,"VRP_CostComponent2") 
  {}
  int ComputeCost(const VRP_State& st) const;
  void PrintViolations(const VRP_State& st, ostream& os = cout) const;
};

*/
/***************************************************************************
 * VRP_Move Neighborhood Explorer:
 ***************************************************************************/

class VRP_MoveDeltaDistanceCost
  : public DeltaCostComponent<VRP_Input,VRP_State,VRP_Move1>
{
public:
  VRP_MoveDeltaDistanceCost(const VRP_Input & in, VRP_DistanceCost& cc) 
    : DeltaCostComponent<VRP_Input,VRP_State,VRP_Move1>(in,cc,"VRP_MoveDeltaDistanceCost") 
  {}
  int ComputeDeltaCost(const VRP_State& st, const VRP_Move1& mv) const;
};

/*
class VRP_MoveDeltaCostComponent2
  : public DeltaCostComponent<VRP_Input,VRP_State,VRP_Move1>
{
public:
  VRP_MoveDeltaCostComponent2(const VRP_Input & in, VRP_CostComponent2& cc) 
    : DeltaCostComponent<VRP_Input,VRP_State,VRP_Move1>(in,cc,"VRP_MoveDeltaCostComponent2") 
  {}
  int ComputeDeltaCost(const VRP_State& st, const VRP_Move1& mv) const;
};
*/
class VRP_MoveNeighborhoodExplorer
  : public NeighborhoodExplorer<VRP_Input,VRP_State,VRP_Move1> 
{
public:
  VRP_MoveNeighborhoodExplorer(const VRP_Input & pin, StateManager<VRP_Input,VRP_State>& psm)  
    : NeighborhoodExplorer<VRP_Input,VRP_State,VRP_Move1>(pin, psm, "VRP_MoveNeighborhoodExplorer") {} 
  void RandomMove(const VRP_State& st, VRP_Move1& mv) const;          
  bool FeasibleMove(const VRP_State& st, const VRP_Move1& mv) const;  
  void MakeMove(VRP_State& st, const VRP_Move1& mv) const;             
  void FirstMove(const VRP_State& st, VRP_Move1& mv) const;  
  bool NextMove(const VRP_State& st, VRP_Move1& mv) const;   
protected:
  //bool AnyNextMove(const VRP_State& st, VRP_Move1& mv) const; //forse basta solo questa, mi dice se c'è una mossa successiva o no
};

/***************************************************************************
 * Output Manager:
 ***************************************************************************/
class VRP_OutputManager
  : public OutputManager<VRP_Input,VRP_Output,VRP_State> 
{
public:
  VRP_OutputManager(const VRP_Input & pin)
    : OutputManager<VRP_Input,VRP_Output,VRP_State>(pin,"VRP_OutputManager") {}
  void InputState(VRP_State&, const VRP_Output&) const;  
  void OutputState(const VRP_State&, VRP_Output&) const; 
}; 
#endif

/*componenti di costo?
se camion eccede capacità e nel caso di quanto eccede
se cliente servito da più camion
se tempo superato non lo consideriamo perché non l'hanno usato nell'esperimento, in caso vediamo più avanti
violazione anche se uso matrice e non ho l'ordine effettivo (forse per questa ragione meglio usare liste frastagliate e non matrice con valore = ordine) (quindi se ordine si ripete per un certo truck, se non viene rispettato quindi mancano numeri)
se metto matrice tipo liste frastagliate ma con 0 devo anche mettere che venga riempito il primo 0 e non quelli dopo
*/

