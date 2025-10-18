/*-- Campervan --*/

#strict 2

#include DOOR

local iFuel;

func Initialize() {
	SetOwner(NO_OWNER);
	SetColorDw(RGBa(255,255,255));
	iFuel = 0;
	return(1);
}

public func CheckFuel(){
	if(iFuel <= 0){
		if(FindContents(OBRL,this()) && FindContents(OBRL,this()) != this()){
			var barrel = FindContents(OBRL,this());
			ChangeDef(BARL,barrel);
			iFuel += 200;
			return(1);
		}
		Sound("StartFail");
		SetComDir(COMD_Stop);
		  FinishCommand();
		return(0);
	}
	return(1);
}

public func CheckOwner(iPlr){
	if(GetOwner() == NO_OWNER) return(1);
	if(Hostile(iPlr, GetOwner())){
		Message("$RideFail1$",GetCursor(iPlr,0));
		GetCursor(iPlr,0)->Sound("CommandFailure1");
		return(0);
	}
	return(1);
}

//movement
public func ContainedLeft(pByObj){
	if(!CheckOwner(GetOwner(pByObj))) return(1);
	if(!CheckFuel()) return(1);
	if(GetAction() != "Walk" && GetAction() != "Idle") return(1);
	if(GetAction() == "Idle") SetAction("Walk");
	SetComDir(COMD_Left);
	return(1);
}

public func ContainedRight(pByObj){
	if(!CheckOwner(GetOwner(pByObj))) return(1);
	if(!CheckFuel()) return(1);
	if(GetAction() != "Walk" && GetAction() != "Idle") return(1);
	if(GetAction() == "Idle") SetAction("Walk");
	SetComDir(COMD_Right);
	return(1);
}


public func ContainedDown(pClonk){
	if(!CheckOwner(GetOwner(pClonk))) return(1);
	if(GetAction() != "Walk" && GetAction() != "Idle") return(1);
	if(GetAction() == "Idle") SetCommand(pClonk,"Exit");
	SetAction("Idle");
	return(1);
}

//refuel
public func ContextRefuel(pByObj){
	[$Refuel$|Image=OBRL]
	AddCommand(pByObj,"Put",this());
	AddCommand(pByObj,"Enter",this());
	AddCommand(pByObj,"Acquire",,,,,,OBRL);
}

//mouse control
protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 if (szCommand == "MoveTo"){
	 ContainedLeft(this());
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 }
 return(0);
}

//cancelled out options so you dont accidentally go to the menu's when driving :)
public func ContainedUp(){ return(1); }
public func ContainedDig(){ return(1); }

public func ContainedUpdate(object self, int comdir, bool dig, bool throw)
{
	if(comdir != COMD_Stop && !CheckOwner(GetOwner(self))) return(0);
	if(iFuel <= 0){
		return(0);
	}
  FinishCommand();
  SetComDir(comdir);

  return(1);
}

//timer
func Reidle(){
	if(GetCon() < 100) return(0);
	if(GetComDir() == COMD_Stop && GetAction() == "Walk" && GetXDir() == 0){
		if(GetDir() == DIR_Right){
			SetXDir(GetXDir()-10);
		}
		SetDir(DIR_Left);
		SetAction("Idle");
	}
	
	//positioning flag vertex
	if(GetDir() == 0 || GetAction() == "Idle"){
		SetVertex(0,0,21,this());
	}else{
		SetVertex(0,0,-21,this());
	}
	
	//smoking :O
	if(GetAction() == "Walk"){
	if(!Random(2)) iFuel--;
	Smoke(GetVertex(0,0,this()),10,RandomX(5,10));
	
		//warn all clonks inside, if fuel is low and there's no more barrels
		if(!FindObject2(Find_ID(OBRL), Find_Container(this()))){
		for(var i in FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this()))){
			if(GetCursor(GetOwner(i),0) != i) continue;
			if(iFuel == 100) PlayerMessage(GetOwner(i), "$FuelWarn50$");
			if(iFuel == 50) PlayerMessage(GetOwner(i), "$FuelWarn25$");
			if(iFuel == 25) PlayerMessage(GetOwner(i), "$FuelWarn10$");
		}
		}
	}
	
	//fuel sign
	if(iFuel <= 0){
		if(GetAction() == "Walk"){
			if(CheckFuel()) return(0);
		}
		if(!FindObject2(Find_ID(OBRL), Find_Container(this())))
		if(!Contained()) Message("{{OBRL}}",this());
	}
	
	//fixing animation bug
	if(GetAction() == "Idle" && GetDir() == 1){
		SetDir(0);
	}
}

public func IsAdvancedProduct(){
	return(1);
}

//sounds

func SoundOpenDoor(){
	Sound("SteelGate1");
}

func SoundCloseDoor(){
	Sound("SteelGate2");
}