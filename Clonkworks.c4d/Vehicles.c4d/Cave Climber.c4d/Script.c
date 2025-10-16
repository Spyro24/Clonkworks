/*-- CaveClimber --*/

#strict 2

#include DOOR

local LegVerts;
local LegList;

func Initialize() {
	LegVerts = [5,6,7,8,9,10];
	LegList = [];
	SetGraphics("Window",this(),GetID(),GFX_Overlay,6);
	SetClrModulation(RGBa(255,255,255),this(),1);
	return(1);
}

//changing graphics & object entrance

func Collection2(pObj){
	if(!CrewMember(pObj)) return(0);
	SetGraphics("Clonk",this());
	SetClrModulation(GetColorDw(pObj),this(),1);
}

func Ejection(pObj){
	if(CrewMember(pObj)){
	SetClrModulation(RGBa(255,255,255),this(),1);
	SetGraphics(0,this());
	}
	
	if(Stuck()){
		Fling(pObj, RandomX(-5,5),-30);
	}
}

func ActivateEntrance(pByObj){
	//Log(GetCommand(pByObj));
	if(GetCommand(pByObj,0) == "Exit") return(_inherited(pByObj));
	if(CrewMember(pByObj) && FindObject2(Find_OCF(OCF_CrewMember), Find_Container(this()))) return(0);
	return(_inherited(pByObj));
}

//control

public func ContainedUp(pByObj){
	[$TxtUp$]
	if(GetBGWall(0,0) && GetAction() == "Idle"){
		Sound("Click");
		SetAction("Movement");
		DeployLegs();
		SetComDir(COMD_Stop);
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
	UndeployLegs();
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

  return(1);
}

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 if (szCommand == "MoveTo"){
	 if(GetAction() == "Idle") ContainedUp(this());
	 if(GetAction() == "Idle") return(0);
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
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
	//if act time is too low, only rotate legs. this is to make sure they dont move when the action just begun.
	if(GetActTime() < 10){
		for(var leg in LegList){
			leg->SetR(Angle(GetX(leg),GetY(leg),GetX(), GetY())-90);
		}
		return(0);
	}
	
	//eject when stuck!
	if(Stuck() || InLiquid()){
		ContainedDownDouble();
	}

	//check legs attached to wall. if 0, fall.
	var attachedAmount = 0;
	for(var lg in LegList){
		attachedAmount += lg->IsAttachedToWall();
	}
	if(attachedAmount < 3 && GetActTime() > 2) ContainedDownDouble(this());
	
	//fixing velocity
	if(!GetCommand()){
	if(GetComDir() == COMD_Stop){
		SetXDir(0); SetYDir(0);
	}
	
	if(GetComDir() == COMD_Up || GetComDir() == COMD_Down){
		SetXDir(0);
	}
	
	if(GetComDir() == COMD_Left || GetComDir() == COMD_Right){
		SetYDir(0);
	}
	}
	
	var usedLegs = 0;
	
	//move legs
	for(var i = 0; i < GetLength(LegList); i++){
		var leg = LegList[i];
		leg->SetR(Angle(GetX(leg),GetY(leg),GetX(), GetY())-90);
		var dist = Distance(GetX(leg)+GetVertex(1,0,leg), GetY(leg)+GetVertex(1,1,leg), GetX()+GetVertex(i,0), GetY()+GetVertex(i,1));
		if(dist > 30 && attachedAmount >= 3){
			if(GetComDir() == COMD_Stop && !GetCommand()) continue;
			if(leg->GetAction() == "MoveLeg" || leg->GetEffect("Legmove",leg)) continue;
			if(usedLegs == 3) continue;
			usedLegs++;
			//realistic-ish leg movement
			var oX, oY;
			oX = GetX()-GetX(leg);
			
			var com;
			com = GetComDir();
			
			if(!GetCommand()){
			if(com == COMD_Left || com == COMD_UpLeft || com == COMD_DownLeft) oX -= 40;
			if(com == COMD_Right || com == COMD_UpRight || com == COMD_DownRight) oX += 40;
			}else{
				if(GetXDir() >= 5) oX += 40;
				if(GetXDir() <= -5) oX -= 40;
			}
			
			oY = GetY()-GetY(leg);
			
			if(!GetCommand()){
			if(com == COMD_Up || com == COMD_UpLeft || com == COMD_UpRight) oY -= 40;
			if(com == COMD_Down || com == COMD_DownLeft || com == COMD_DownRight) oY += 40;
			}else{
				if(GetYDir() >= 5) oY += 40;
				if(GetYDir() <= -5) oY -= 40;
			}
			
			leg->MoveToPlace(oX+GetVertex(i,0),oY+GetVertex(i,1));
		}
	}
}

//deployment and unemployment of legs
protected func DeployLegs(){
	//lets avoid duplicates
	for(var leg in LegList){
		RemoveObject(leg);
	}
	
	for(var i in LegVerts){
		var newLeg = CreateObject(CCLG, GetVertex(i,0),GetVertex(i,1));
		LegList[GetIndexOf(i,LegVerts)] = newLeg;
	}
}

protected func UndeployLegs(){
	for(var leg in LegList){
		RemoveObject(leg);
	}
	LegList = [];
}

private func legrem(rem){
	RemoveObject(rem);
}

// Reject content menu
public func RejectContents(){ return(true); }