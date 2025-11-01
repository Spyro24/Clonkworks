/*-- T-Flint --*/

#strict 2
#include TFLN

local iLastController; // Fuer Killverfolgung: Letzter Controller vor dem Einsammeln

protected func Hit() {
  Sound("ArrowHit");
  if(!ActIdle()) return true;
  Sound("Fuse");
  SetAction("Activated");
  // Picture anpassen
  SetPicture(0, 12 + 64, 64, 64);
  return true;
}

public func Activate(pClonk)
{
 [$TxtFuse$]
 if(!ActIdle()) return true;
 Sound("Fuse");
 SetAction("Activated");
 iLastController = GetController();
 // Picture anpassen
 SetPicture(0, 12 + 64, 64, 64);
 
 // Attaching to ceilings and walls
 if(GetProcedure(pClonk) == "SCALE"){
	 var steps = 10; //max steps
	 var dir = GetDir2(pClonk);
	 Exit();
	 while(steps > 0 || !GBackSolid()){
		 SetX(GetX()+dir);
		 steps--;
	 }
	 return true;
 }
 
 if(GetProcedure(pClonk) == "HANGLE"){
	 var steps = 10; //max steps
	 Exit();
	 while(steps > 0 || !GBackSolid()){
		 SetY(GetY()-1);
		 steps--;
	 }
	 return true;
 }
 
 return true;
}
