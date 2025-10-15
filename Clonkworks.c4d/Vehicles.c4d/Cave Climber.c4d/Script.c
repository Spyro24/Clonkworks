/*-- CaveClimber --*/

#strict 2

#include DOOR

func Initialize() {
	SetGraphics("Player",this(),GetID(),GFX_Overlay,1);
	SetClrModulation(RGBa(0,0,0,0),this(),1);
	return(1);
}

//changing graphics

func Collection2(pObj){
	if(!CrewMember(pObj)) return(0);
	SetGraphics("Clonk",this());
	SetClrModulation(GetColorDw(pObj),this(),1);
}

func Ejection(){
	SetClrModulation(RGBa(0,0,0,0),this(),1);
	SetGraphics(0,this());
}

func RejectEntrance(pByObj){
	if(CrewMember(pByObj)) return(0);
	return(1);
}

//control

public func ContainedUp(pByObj){
	[$TxtUp$]
	if(GetBGWall(0,0) && GetAction() == "Idle"){
		Sound("Click");
		SetAction("Movement");
		return(1);
	}
	
	if(GetAction() == "Idle"){
		Sound("Error");
		return(1);
	}
	
	if(GetPlrJumpAndRunControl(pByObj->GetController())) return(1);
	
	if(GetAction() == "Movement"){
		SetComDir(COMD_Up);
		return(1);
	}
}

public func ContainedDown(pByObj){
	[$TxtDown$]
		if(GetAction() == "Idle") return(0);
	if(GetPlrJumpAndRunControl(pByObj->GetController())) return(1);
	if(GetAction() == "Movement"){
		SetComDir(COMD_Down);
	}
	
	return(1);
}

public func ContainedLeft(pByObj){
	[$TxtLeft$]
	if(GetPlrJumpAndRunControl(pByObj->GetController())) return(1);
	if(GetAction() == "Movement"){
		SetComDir(COMD_Left);
	}
	
	return(1);
}

public func ContainedRight(pByObj){
	[$TxtRight$]
	if(GetPlrJumpAndRunControl(pByObj->GetController())) return(1);
	if(GetAction() == "Movement"){
		SetComDir(COMD_Right);
	}
	
	return(1);
}

public func ContainedDig(pByObj){
	[$TxtDig$]
	if(GetPlrJumpAndRunControl(pByObj->GetController())) return(1);
	if(GetAction() == "Movement"){
		SetComDir(COMD_Stop);
	}
	
	return(1);
}

public func ContainedDownDouble(pByObj){
	[$TxtDownDouble$]
	if(GetAction() != "Movement") return(0);
	Sound("Click");
	SetAction("Idle");
}

//jump'n'run controls

public func ContainedUpdate(object self, int comdir, bool dig, bool throw)
{
	
  FinishCommand();
  SetComDir(comdir);
  if(GetAction() != "Movement"){
	  SetComDir(COMD_Stop);
	  return(0);
  }
  if(comdir == 0){
	  SetXDir(0); SetYDir(0);
  }

  return(1);
}

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 if (szCommand == "MoveTo"){
	 if(GetAction() == "Idle") ContainedUp(this());
	 if(GetAction() == "Idle") return(0);
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 }
 
  if (szCommand == "Dig"){
	 if(GetAction() == "Idle") ContainedUp(this());
	 if(GetAction() == "Idle") return(0);
  return(SetCommand(this(),"MoveTo", pTarget, iTx, iTy,,true));
 }
 
 return(0);
}

protected func ControlCommandFinished(){
		  SetComDir(COMD_Stop);
		  SetXDir(0); SetYDir(0);
}


//global functions

global func GetBGWall(iX,iY){
	var mat = GetMaterial(iX,iY);
	if(mat == Material("Sky")) return(0);
	if(GetMaterialVal("Density", "Material", mat) == 0) return(1);
}

//sounds

func SoundOpenDoor(){
	Sound("Airlock1");
}

func SoundCloseDoor(){
	ScheduleCall(this(),"SoundCloseDoorScheduled",7,,"Airlock2");
}

func SoundCloseDoorScheduled(sound){
	Sound(sound);
}

//PhaseCall

protected func MoveLegs(){
	if(!GetBGWall(0,0)) ContainedDownDouble(this());
	
	//bugged
/* 	
	if(GBackSolidAtCircle(GetX(),GetY(),20)){
	DigFree(GetX(),GetY(),20);
	Sound("Dig*");
	} */
}

// Reject content menu
public func RejectContents(){ return(true); }