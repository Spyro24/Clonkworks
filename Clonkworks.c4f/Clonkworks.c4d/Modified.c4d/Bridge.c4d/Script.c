/*-- Brückensegment --*/

#strict
#include BCON

local is_locked;	// Ist im Boden verankert?
local locktype_attach; // is Attached instead of locked?
local PrimaryAttachment; //what object does this depend on while attached?

local paX, paY;

public func CanAttachBridge(){
	if(!ObjectCount(BCNR)) return(0);
	if(is_locked && locktype_attach) return(1);
} 
public func LeftVert(){ return(4); }
public func RightVert(){ return(5); }

protected func Initialize() {
  // Im Freien erzeugte Brückensegmente sind verankert
  if (!Contained())
    Lock(1, 1);
  return(1);
}

protected func Entrance() {
  // Im Behälter: lösen
  Release(1);
  return(1);
}

public func ControlDigDouble(caller) {
  [$TxtLock$]
  
  //attachment behaviour
  if(ObjectCount(BCNR) && !is_locked){ //later will change to a rule check
	var Attachable;
	Attachable = FindObject2(Find_Category(C4D_Vehicle), Find_AtPoint(0,0), Find_Func("CanAttachBridge"), Find_NoContainer());
	if(!Attachable) Attachable = FindObject2(Find_Category(C4D_Vehicle), Find_AtPoint(0,10), Find_Func("CanAttachBridge"), Find_NoContainer()); //not on exact position? why not check the bottom?
	if(Attachable){ //finding and setting the attachable object
		//left or right?
		if(GetX() <= GetX(Attachable) && Attachable->CanAttachLeft()){
			DecollideAll();
			is_locked = 1;
			locktype_attach = 1;
			LocalN("iAttachedLeft",Attachable) = this();
			iAttachedRight = Attachable;
			PrimaryAttachment = Attachable;
			SetAction("Attached", Attachable);
			SetActionData(256*RightVert() + Attachable->LeftVert(), this());
			SetSolidMask(0, 11, 72, 3);
			Sound("Connect");
			return(1);
		}else if(GetX() > GetX(Attachable) && Attachable->CanAttachRight()){
			DecollideAll();
			is_locked = 1;
			locktype_attach = 1;
			LocalN("iAttachedRight",Attachable) = this();
			iAttachedLeft = Attachable;
			PrimaryAttachment = Attachable;
			SetAction("Attached", Attachable);
			SetActionData(256*LeftVert() + Attachable->RightVert(), this());
			SetSolidMask(0, 11, 72, 3);
			Sound("Connect");
			return(1);
		}
	}
  }
  
  // Wenn verankert lösen
  if (is_locked) Release();
  // sonst verankern
  else Lock();
  return(1);
}

private func DecollideAll(){
	var x = 0;
	while(x < GetVertexNum()-2){
		SetVertex(x,2,GetVertex(x,2)+64,this(),2);
		x++;
	}
}

private func SetAttachPos(){
	paX = GetX(PrimaryAttachment) - GetX();
	paY = GetY(PrimaryAttachment) - GetY();
	SetAction("AttachedLocal");
}

private func SetAttach(){
	//getting object on top to update
	var ObjOnTop;
	ObjOnTop = FindObjects( Find_Or(Find_Category(C4D_Vehicle), Find_Category(C4D_Living), Find_Category(C4D_Object)), Find_NoContainer(), Find_OnLine(-34,-7,34,-7), Find_Exclude(this()), Find_Exclude(PrimaryAttachment));
	//Log("Amount of objects on top of %s: %d", GetName(), GetLength(ObjOnTop));
	var bX = GetX();
	var bY = GetY();
	
	//updating velocity and position if needed
	if(!PrimaryAttachment) Release();
	if(GetX() != GetX(PrimaryAttachment)-paX || GetX() != GetY(PrimaryAttachment)-paY) SetPosition(GetX(PrimaryAttachment)-paX, GetY(PrimaryAttachment)-paY);
	if(GetXDir() != GetXDir(PrimaryAttachment)) SetXDir(GetXDir(PrimaryAttachment));
	if(GetYDir() != GetYDir(PrimaryAttachment)) SetYDir(GetYDir(PrimaryAttachment));
	//fixing some of the sliding
	if(GetXDir() > 1) SetXDir(GetXDir() - 1);
	if(GetXDir() < 1) SetXDir(GetXDir() + 1);
	//Setting positions of objects on top.
	var Xdif = GetX()-bX;
	var Ydif = GetY()-bY;
	for(var i in ObjOnTop){
		SetPosition(GetX(i)+Xdif,GetY(i), i);
	}
	
	for(var j in ObjOnTop){
		SetPosition(GetX(j),GetY(j)+Ydif, j);
	}
	
	//collision detection
	if(GetXDir() < 0 && GBackSolid(-37,3)){
		SetXDir(0);
		DisturbConnections(1, 0, 0);
	}
	
	if(GetXDir() > 0 && GBackSolid(37,3)){
		SetXDir(0);
		DisturbConnections(0, 0, 0);
	}
}

private func DisturbConnections(int lr, int x, int y){
	if(lr == 0 && iAttachedLeft){
		iAttachedLeft->SetXDir(x);
		iAttachedLeft->SetYDir(y);
		iAttachedLeft->~DisturbConnections(lr,x,y);
	}else if(lr == 1 && iAttachedRight){
		iAttachedRight->SetXDir(x);
		iAttachedRight->SetYDir(y);
		iAttachedRight->~DisturbConnections(lr,x,y);
	}
}

private func Lock(quiet, dont_descend) {
  SetAction("Locked");
  SetSolidMask(0, 11, 72, 3);
  is_locked = 1;

  if (!quiet)
    Sound("Connect");

  // Verankern
  if (!dont_descend)
    if (!GBackSolid(0, 10))
      SetPosition(GetX(), GetY()+10);
  return(1);
}

private func Release(quiet, dont_ascend) {
  if(iAttachedLeft){
	  if(iAttachedLeft == PrimaryAttachment) PrimaryAttachment = 0;
	  iAttachedLeft->ClearRight();
	  iAttachedLeft->~Release();
	  iAttachedLeft = 0;
  }
   if(iAttachedRight){
	  if(iAttachedRight == PrimaryAttachment) PrimaryAttachment = 0;
	  iAttachedRight->ClearLeft();
	  iAttachedRight->~Release();
	  iAttachedRight = 0;
  }
  
  if(locktype_attach && this()){
	  var new = CreateObject(GetID());
	  new->Release();
	  RemoveObject(this());
	  return(0);
  }
  
  SetAction("Idle");
  SetSolidMask();
  is_locked = 0;

  if (!quiet)
    Sound("Connect");

  // Aus der Erde lösen
  if (!dont_ascend) {
    var ascend_pixels = 0;
    while (Stuck() && (++ascend_pixels < 12))
      SetPosition(GetX(), GetY()-1);
  }

  return(1);
}

public func IsBridge() { return(1); }

public func IsLocked() { return(is_locked); }

// Nicht an die Dampflok koppeln lassen
public func NoPull()
{
  return IsLocked();
}

public func RejectEntrance(){
	return(is_locked);
}