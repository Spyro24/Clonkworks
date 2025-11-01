/*-- Tera-Flint --*/

#strict

#include FLNT
local exploder;

/* Aufschlag */

public func ExplodeSize() { return(25); }

/* Forschung */

public func GetResearchBase() { return(EFLN); }

protected func Hit()
{
  exploder = CreateObject(FLNT);
  exploder->Explode(ExplodeSize());
  SetAction("Exploding");
  SetXDir(0);
  SetYDir(0);
  var size = ExplodeSize() - 5;
  var timer = 10;
  while(size > 0){
	  Schedule(Format("LocalN(\"exploder\") = CreateObject(FLNT,RandomX(-45,45),RandomX(-45,45))"),timer,0);
	  Schedule(Format("LocalN(\"exploder\")->~Explode(%d)",size),timer+1,0);
	  timer+=RandomX(5,10);
	  size -= 5;
  }
  Schedule("RemoveObject()",timer+15);
}

public func RejectEntrance(idObj){
	if(GetAction() eq "Exploding") return(1);
}