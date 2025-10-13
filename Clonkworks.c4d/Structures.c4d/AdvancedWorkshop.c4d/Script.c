/*-- Advanced Workshop --*/

#strict

#include CXEC
#include DOOR
#include WRKS
#include BS35

func Initialize() {
return(1);
}

private func SoundOpenDoor()
{
  Sound("SteelGate2");
}
  
private func SoundCloseDoor()
{
  Sound("SteelGate2");
}

/* Produkteigenschaften (überladbar) */
func ProductType() { return(C4D_Vehicle()); }
func ProductCondition() { return(); }

/* Produktion */

public func IsProducerOf(caller, def) 
{
  if(!def->~IsAdvancedProduct()) return(0);
  if(!def->~IsAnvilProduct() && !(GetCategory(,def) & C4D_Vehicle())) return(0);
  if (GetComponent(DUMM, , , def) > 0) return (0);
  if (!IsBuilt ()) return (0);
  if (!GetPlrKnowledge (GetOwner (caller), def)) return (0);
  if (ProductCondition ())
    if (!DefinitionCall (def, ProductCondition ()))
      return (0);
  //Suche nach besserer...
  if (NeedsEnergy ())
    if (FindSuppliedObjectCloseTo (caller)) return (0);
  return(1);
}

func FindSuppliedObjectCloseTo (obj, def) {
  var obj2;
  if (!def) def = GetID ();
  while (obj2 = FindObject (def, (GetX (obj) - 1000) - GetX (), (GetY (obj) - 500) - GetY (), 2000, 1000,
    OCF_Fullcon (), 0, 0, 0, obj2))
      if (FindObject (PWRL, 0,0,0,0, 0,"Connect",obj2))
        return(obj2);
  return(0);
}

public func HowToProduce (clonk, def) {
  if(NeedsEnergy())
  { 
    AddCommand (clonk, "Call", this (), def, 0, 0, 0, "HowToProduce");
    AddCommand (clonk, "Energy", this ());
    return(1);    
  }
  AddCommand (clonk, "Wait", 0, 0, 0, 0, 10);
  AddCommand (clonk, "Call", this (), def, 0, 0, 0, "StartProduction");
  AddCommand (clonk, "Enter", this ());
  return(1);
}

public func MenuProductionUtility(Foo, pWorker) {
  if (!ActIdle()) return(0);
  CreateMenu(CXCN, pWorker, this(), 1, "$NoPlrKnowledge$");
  var i = 0, def; SetVar(0,-1);
  while (def = GetPlrKnowledge(GetOwner(pWorker), 0, i++, C4D_Object()))
    if (def->~IsAnvilProduct() && def->~IsAdvancedProduct())
      AddMenuItem("$Construction$: %s","SelectProduction",def,pWorker,0,pWorker);
  return(1);
}

private func MenuProductionVehicle(Foo, pCaller) {
  CreateMenu(CXCN,pCaller,this(),1,"$NoPlrKnowledge$");
  for(var i=0,idKnowledge; idKnowledge=GetPlrKnowledge(GetOwner(pCaller),0,i,ProductType ()); ++i)
  {
    if(ProductCondition())
      if(!DefinitionCall(idKnowledge, ProductCondition()))
        continue;
	if(GetComponent(DUMM, , , idKnowledge) > 0)
		continue;
	if(!idKnowledge->~IsAdvancedProduct()) continue;
    AddMenuItem("$Construction$: %s", "SelectProduction", idKnowledge, pCaller, 0, pCaller);
  }
  return(1);
}

public func SelectProduction(idType,pWorker,bSpecial2) {
  AddCommand(pWorker,"Call",this(),idType,bSpecial2,0,0,"StartProduction", 0, 1);
  AddCommand(pWorker,"Enter",this());
  return(1);
}
  
public func StartProduction(pWorker,idType,bSpecial2) {
  var pToBuild;
  pToBuild=FindIncompleteContents(idType);
  if(!pToBuild)
    if(!(pToBuild=CreateConstruction(idType,0,0,GetOwner(),1))) return(0);
  if (Contained(pToBuild)!=this()) 
    Enter(this(),pToBuild);
  pToBuild->~OnStartProduction(this);
  AddCommand(pWorker,"Build",pToBuild, 0,0,0,0,0,0, 3);
  if(bSpecial2)
    AppendCommand(pWorker,"Call",this(),idType,bSpecial2,0,0,"ProductionComplete", 0, 1);
  return(1);
}

public func ProductionCompleteFailed(pWorker,idType,bSpecial2) {
  AddCommand(pWorker,"Build",FindIncompleteContents(idType)); 
  return(1);
}

public func ProductionComplete(pWorker,idType,bSpecial2) {
  if(!bSpecial2) return(0);
  AppendCommand(pWorker,"Wait",0,0,0,0,35);
  AppendCommand(pWorker,"Call",this(),idType,bSpecial2,0,0,"StartProduction");
  return(1);
}

private func MenuProduction(pCaller){
	CreateMenu(CXCN,pCaller,this(),1,"");
	AddMenuItem("$Category$: %s", "MenuProductionUtility", A_S1, pCaller, 0, pCaller);
	AddMenuItem("$Category$: %s", "MenuProductionVehicle", A_S2, pCaller, 0, pCaller);
}

public func ContextConstruction(pCaller) {
  [$Production$|Image=CXCN|Condition=IsBuilt]
  return(MenuProduction(pCaller));
}

global func AdvancedResearchAllowed(iPlr){
	var looky = FindObject2( Find_Or(Find_Owner(iPlr), Find_Allied(iPlr)), Find_ID(ADVW), Find_OCF(OCF_Fullcon));
	if(looky) return(1);
	else return(0);
}

protected func IsBuilt() {
  return(GetCon() >= 100);
}


protected func ContainedUp(pCaller) 
{
  [$Production$|Image=CXCN]
  return(MenuProduction(pCaller));
}


private func CheckBuild() {
	
	for(var thing in FindObjects(Find_Container(this()))){
		if(thing ->~ ExitWorkshop() == true){
			SetCommand(thing, "Exit");
		}
	}
	
  var bWorkingClonk=IsWorking();
  var bBuildingAction=(GetAction()S="Build");
  if(bWorkingClonk  && !bBuildingAction) if (ActIdle()) SetAction("Build");
  if(!bWorkingClonk && bBuildingAction ) {
    SetAction("Idle");
    EnergyCheck(0);
  }
  return(1);
}

private func IsWorking() {
  if (!Contents()) return(0);
  return(FindObject(0,0,0,0,0,0,"Build",0,this()));
}

private func Smoking() {  
  if (GetPhase()%3) return(1);
  if (Random(6)) Smoke(+48,-25,8);
  if (Random(8)) Smoke(43,-28,5+Random(3));
  
  if (Random(6)) Smoke(+19,-25,8);
  if (Random(8)) Smoke(13,-28,5+Random(3));
  
  if (Random(6)) Smoke(+28,-23,8);
  if (Random(8)) Smoke(22,-20,5+Random(3));
  return(1);
}



private func FindIncompleteContents(idSearched) {
  for(var i=0,pContent; pContent=Contents(i); ++i)
    if(GetID(pContent)==idSearched)
      if(GetCon(pContent)<100)
        return(pContent);
  return(0);
}
