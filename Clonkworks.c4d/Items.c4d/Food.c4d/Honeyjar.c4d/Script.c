/*-- Neues Script --*/

#strict 2
#include ARWP

public func UnpackTo() { return(HONY); }
public func IsArrowPack() { return(0); }
public func MaxPackCount() { return(5); }
public func IsAnvilProduct() { return(0); }

public func Activate(pClonk){
	[$TxtDrink$]
	Sound("Drink");
	pClonk->Drink(this());
	DoEnergy(20,pClonk);
	
	DoPackCount(-1);
	
	UpdatePicture();
	UpdateMass();
}

public func CalcValue(object pInBase, int iForPlayer)
{
  // Je nach Anzahl
  return((GetValue(0, GetID())*PackCount()/MaxPackCount())+2);
}

public func DoPackCount(iChange) 
{
  if(iChange>0) Sound("OpenJar");
  iUsedItems-=iChange;
  if(PackCount()<=0) return(RemoveObject());
  UpdatePicture();
  // Masse anpassen
  UpdateMass();
}

public func Hit(){
	if(!Random(3)) Sound("Crystal*");
}