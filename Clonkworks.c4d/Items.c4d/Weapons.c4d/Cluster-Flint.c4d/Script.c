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
  var i = 5;
  var timer = 10;
  var range = 5;
  while(i > 0){
	  var iX, iY, Size;
	  iX = RandomX(-range,range);
	  iY = RandomX(-range,range);
	  Size = 25 - (Distance(0,0,iX,iY)/2);
	  Schedule(Format("DoClusterExplosion(%d)",range),timer,0);
	  timer+=RandomX(5,10);
	  i--;
	  range+=5;
  }
  Schedule("RemoveObject()",timer+15);
}

public func RejectEntrance(idObj){
	if(GetAction() eq "Exploding") return(1);
}

func DoClusterExplosion(range){
	  var exploder = CreateObject(FLNT,RandomX(-45,45),RandomX(-45,45));
	  var iX, iY, Size;
	  iX = RandomX(-range,range);
	  iY = RandomX(-range,range);
	  Size = 25 - (Distance(0,0,iX,iY)/2);
	  exploder->Explode(Size);
}