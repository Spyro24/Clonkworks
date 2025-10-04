/*-- Neues Script --*/

#strict 2
#include CPBT
#include BAS2

//TODO: Overhaul this whole object

public func LightningStrike() 
{
  if (GetCon() < 100) return(0);
    Sound("Electric");
	SetBatteryEnergy(100);
	CastParticles("PxSpark", 10,100, 0,-30, 25, 80, RGBa(255,255,0,0), RGBa(255,149,0,127));
}

public func GetChargeHold(){ return(30); }