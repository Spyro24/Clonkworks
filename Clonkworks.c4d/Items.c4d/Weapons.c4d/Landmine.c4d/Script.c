/*-- T-Flint --*/

#strict 2
#include TFLS

local iLastController; // Fuer Killverfolgung: Letzter Controller vor dem Einsammeln
local AboutToExplode; //tells this to stop detection when about to blow up.
local TimerDebt; //when attaching to a wall, this is an extra set of seconds so you have time to get away.

protected func Hit() {
  if(!ActIdle()) return true;
	return(Attachment());
}

public func Attachment(){
   SetPicture(0, 7 + 64, 64, 64);
   Sound("Click");
   SetAction("Active");
}

public func Activate(pClonk)
{
 if(!ActIdle()) return true;
 
 // Attaching to ceilings and walls
 if(GetProcedure(pClonk) == "SCALE"){
	 var steps = 10; //max steps
	 var dir = GetDir2(pClonk);
	 Exit();
	 while(steps > 0 && !GBackSolid()){
		 SetX(GetX()+dir);
		 steps--;
	 }
	 SetX(GetX()+dir);
	 TimerDebt += 36;
	 //return true;
 }
 
 if(GetProcedure(pClonk) == "HANGLE"){
	 var steps = 10; //max steps
	 Exit();
	 while(steps > 0 && !GBackSolid()){
		 SetY(GetY()-1);
		 steps--;
	 }
	 SetY(GetY()-1);
	 TimerDebt += 36;
	 //return true;
 }
 
 Exit(this(), 0, (GetDefBottom(pClonk)-GetY())+2);
 return(Attachment());
}

public func Stickiness(){
/* 	if((GetContact(this(),-1) & 8)&& GetAction() == "Activated") SetActionKeepPhase("ActivatedStick");
	if(!(GetContact(this(),-1) & 8) && GetAction() == "ActivatedStick") SetActionKeepPhase("Activated"); */
}

public func CheckNearby(){
	if(!(GetContact(this(),-1) & 8) || FindObject2(Find_NoContainer(), Find_ID(SCWD), Find_Distance(5))){
		Sound("Click");
		SetPicture(0, 7, 64, 64);
		SetAction("Idle");
		AboutToExplode = false;
		SetColorDw(GetPlrColorDw(GetOwner()));
	}
	
	if(GetActTime() < 36){
		return(0);
	}
	if(TimerDebt > 0){
		TimerDebt--;
		return(0);
	}
	
	/* if(GetActTime() == 36*3) Sound("Ding"); */
	
	if(FindObject2(Find_Distance(MineRange()), Find_NoContainer(), Find_OCF(OCF_Alive)) && !AboutToExplode){
		SetColorDw(RGBa(255,0,0));
		Sound("Minebeep");
		Schedule("DoMineExplosion()",36/2,0);
		AboutToExplode = true;
	}
}

func DoMineExplosion(){
	if(!AboutToExplode) return(0);
	Explode(ExplosionSize());
}

public func ExplosionSize(){ return(27); }

public func MineRange(){ return(ExplosionSize()); }

public func RejectEntrance(){
	if(GetAction() != "Idle") return(1);
}

public func IsAdvancedProduct(){ return(1); }