/*-- Neues Script --*/

#strict
#include MDRL
#include DUMM

local Color;
local Mass;
local Speed;
local Power;
local Effect;

func FRGUpdate(){
	//SetColorDw(Color, this());
	SetMass(50*Mass, this());
	SetClrModulation(Color, this());
}

protected func SignalDelay()
{
	if(GetAction() eq "Idle") SetAction("Walk");
	
	var x;
	if(GetDir() == 0) x = -25;
	else x = 25;
	
	if(PoweredEnough()){
	if(GetMaterial(x,0) != Material("Sky") && GetMaterial(x,0) != Material("Tunnel") && Abs(GetXDir()) != 0){
		Drill(x);
	}else if(GetMaterial(x,5) != Material("Sky") && GetMaterial(x,5) != Material("Tunnel") && Abs(GetXDir()) != 0){
		Drill(x);
	}else if(GetMaterial(x,-5) != Material("Sky") && GetMaterial(x,-5) != Material("Tunnel") && Abs(GetXDir()) != 0){
		Drill(x);
	}
  }
  
  DoEnergy(-1);
  
  // Keine Wartezeit
  if (!iWait) return(0);
  // Wartezeit aufzählen
  --iWait;
  // Abfahrtszeit vorm Zählerende
  if (iWait == 2) DepartFromSignal();
  return(1);
}

protected func Drill(int x){
		x = GetX()+x;
			var y;
			y = GetY();
		/* FreeRect(GetX(), GetY()-20, 25,30);
		FreeRect(GetX()+x, GetY()-20, 10,30); */
		BlastFree(x,(y-2)-(Power/2),16+Power,GetOwner()-1);
		Sound("Drill");
}

private func AdjustTrainSpeed()
{
   SetPhysical("Walk",(Speed*(Speed-Power))*15, 2);
}

public func IsAdvancedProduct(){ return(1); }
public func GetResearchBase() { return(MDRL); }