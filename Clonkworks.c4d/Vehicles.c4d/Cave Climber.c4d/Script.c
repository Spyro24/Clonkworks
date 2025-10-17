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
		Fling(pObj, RandomX(-5,5),-5);
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
		Message("$ErrNoWall$",this());
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

//leg pos fixer
public func UnstuckLegs(){
	for(var leg in LegList){
		if(GBackSolid(GetVertex(0,0,leg),GetVertex(1,0,leg)) || GBackLiquid(GetVertex(0,0,leg),GetVertex(1,0,leg))){
			if(GetX(leg) < GetX()) SetX(GetX(leg)+3,leg);
			if(GetX(leg) > GetX()) SetX(GetX(leg)-3,leg);
			
			if(GetY(leg) > GetY()) SetY(GetY(leg)+3,leg);
			if(GetY(leg) < GetY()) SetY(GetY(leg)-3,leg);
			return(1);
		}
		
		if(GetMaterial() != -1 && leg->GetBGWall(GetVertex(0,0,leg),GetVertex(1,0,leg)) == 0){
			if(GetX(leg) < GetX()) SetX(GetX(leg)+5,leg);
			if(GetX(leg) > GetX()) SetX(GetX(leg)-5,leg);
			
			if(GetY(leg) > GetY()) SetY(GetY(leg)-5,leg);
			if(GetY(leg) < GetY()) SetY(GetY(leg)+5,leg);
			return(1);
		}
	}
}

//PhaseCall

protected func MoveLegs(){
	UnstuckLegs();
	
		//rotation
		for(var leg in LegList){
			leg->SetR(Angle(GetX(leg),GetY(leg),GetX(), GetY())-90);
		}
		
	//if act time is too low, only rotate legs. this is to make sure they dont move when the action just begun.
	if(GetActTime() < 10){
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
	if(attachedAmount < 2) ContainedDownDouble(this());
	
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
	
	var BannedLegs = []; //legs that can not be used.
	var MovedLegs = 0; //Legs that are currently moving
	
	//dont move legs when youre in the sky.
	if(GetMaterial(0,0) == -1) return(0);
	
	//set moved legs to current moving legs
	for(var l in LegList){
		if(l->IsAttachedToWall()) continue;
		MovedLegs++;
	}
	
	//move legs
	for(var i = 0; i < GetLength(LegList); i++){
		if(MovedLegs > 1) break; //max legs
		var leg = LegList[i];
		leg->SetR(Angle(GetX(leg),GetY(leg),GetX(), GetY())-90);
		var dist = Distance(GetX(leg)+GetVertex(1,0,leg), GetY(leg)+GetVertex(1,1,leg), GetX()+GetVertex(i,0), GetY()+GetVertex(i,1));
		if(!Random(10) && dist > 30){
			if(GetComDir() == COMD_Stop && !GetCommand()) continue;
			if(leg->GetAction() == "MoveLeg" || leg->GetEffect("Legmove",leg)) continue;
			//realistic-ish leg movement
			var oX, oY;
			oX = GetX()-GetX(leg);
			
			var com;
			com = GetComDir();
			
			if(!GetCommand()){
			if(com == COMD_Left || com == COMD_UpLeft || com == COMD_DownLeft) oX -= 40;
			if(com == COMD_Right || com == COMD_UpRight || com == COMD_DownRight) oX += 40;
			}else{
				if(GetXDir() >= 5){
					com = COMD_Right;
					oX += 40;
				}
				if(GetXDir() <= -5){ 
					com = COMD_Left;
					oX -= 40;
				}
			}
			
			oY = GetY()-GetY(leg);
			
			if(!GetCommand()){
			if(com == COMD_Up || com == COMD_UpLeft || com == COMD_UpRight) oY -= 40;
			if(com == COMD_Down || com == COMD_DownLeft || com == COMD_DownRight) oY += 40;
			}else{
				if(GetYDir() >= 5){ 
					if(com == COMD_Left) com = COMD_DownLeft;
					if(com == COMD_Right) com = COMD_DownRight;
					oY += 40;
				}
				if(GetYDir() <= -5){
					if(com == COMD_Left) com = COMD_UpLeft;
					if(com == COMD_Right) com = COMD_UpRight;
					oY -= 40;
				}
			}
			
			//banning legs, only one leg per side. going through all 6 legs.
			if(i == 0){
				if(InArray(leg->GetLegVert(), BannedLegs) == -1){
				if(com == COMD_Up || com == COMD_Down) ArrayAdd(BannedLegs,8);
				if(com == COMD_Left || com == COMD_Right) ArrayAdd(BannedLegs,7);
				if(com == COMD_UpLeft || com == COMD_UpRight || com == COMD_DownLeft || com == COMD_DownRight) ArrayAdd(BannedLegs,10);
				}
			}
			
			if(i == 1){
				/* if(com == COMD_Up || com == COMD_Down) ArrayAdd(BannedLegs,9);
				if(com == COMD_Left || com == COMD_Right) ArrayAdd(BannedLegs,9);
				if(com == COMD_UpLeft || com == COMD_UpRight || com == COMD_DownLeft || com == COMD_DownRight) ArrayAdd(BannedLegs,10); */
				if(InArray(leg->GetLegVert(), BannedLegs) == -1){
				ArrayAdd(BannedLegs,9);
				}
			}
			
			if(i == 2){
				if(InArray(leg->GetLegVert(), BannedLegs) == -1){
				if(com == COMD_Up || com == COMD_Down) ArrayAdd(BannedLegs,10);
				if(com == COMD_Left || com == COMD_Right) ArrayAdd(BannedLegs,5);
				if(com == COMD_UpLeft || com == COMD_UpRight || com == COMD_DownLeft || com == COMD_DownRight) ArrayAdd(BannedLegs,8);
				}
			}
			
			if(i == 3){
				if(InArray(leg->GetLegVert(), BannedLegs) == -1){
				if(com == COMD_Up || com == COMD_Down) ArrayAdd(BannedLegs,5);
				if(com == COMD_Left || com == COMD_Right) ArrayAdd(BannedLegs,10);
				if(com == COMD_UpLeft || com == COMD_UpRight || com == COMD_DownLeft || com == COMD_DownRight) ArrayAdd(BannedLegs,7);
				}
			}
			
			if(i == 4){
				if(InArray(leg->GetLegVert(), BannedLegs) == -1){
				ArrayAdd(BannedLegs,6);
				}
			}
			
			if(i == 5){
				if(InArray(leg->GetLegVert(), BannedLegs) == -1){
				if(com == COMD_Up || com == COMD_Down) ArrayAdd(BannedLegs,7);
				if(com == COMD_Left || com == COMD_Right) ArrayAdd(BannedLegs,8);
				if(com == COMD_UpLeft || com == COMD_UpRight || com == COMD_DownLeft || com == COMD_DownRight) ArrayAdd(BannedLegs,5);
				}
			}
			
			if(InArray(leg->GetLegVert(), BannedLegs) != -1) continue;
			if(com == COMD_Stop) continue;
			
			leg->MoveToPlace(oX+GetVertex(i,0),oY+GetVertex(i,1));
			MovedLegs++;
		}
		
		//DebugLog("TOTAL MOVED LEGS ON FRAME: %d",MovedLegs); <-- lagfest {{WIPF}}
	}
}

//deployment and unemployment of legs
protected func DeployLegs(){
	var vx = GetXDir();
	var vy = GetYDir();
	//lets avoid duplicates
	for(var leg in LegList){
		RemoveObject(leg);
	}
	
	for(var i in LegVerts){
		var newLeg = CreateObject(CCLG, GetVertex(i,0),GetVertex(i,1));
		LegList[GetIndexOf(i,LegVerts)] = newLeg;
		newLeg->SetLegVert(i);
		
		var Rope = CreateObject(CK5P);
		Rope->ConnectObjects(this(),newLeg);
		LocalN("RopeColor",Rope) = RGBa(50,50,50);
		LocalN("fNoPickUp_0",Rope) = 1;
		LocalN("fNoPickUp_1",Rope) = 1;
		LocalN("iVtx2",Rope) = 1;
		Rope->SetPoint(1, GetX(newLeg)-GetVertex(1,0,newLeg), GetY(newLeg)-GetVertex(1,1,newLeg));
		Rope->SetRopeLength(100);
		
		//is leg ok to place?
		if(newLeg->IsAttachedToWall() == false){
			SetXDir(vx); SetYDir(vy);
			UndeployLegs();
			Message("$ErrNoLegSpace$",this());
			Sound("Error");
			ContainedDownDouble();
			return(0);
		}
	}
	
	SetXDir(vx); SetYDir(vy);
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

//Advanced research and crafting
public func IsAdvancedProduct(){ return(1); }