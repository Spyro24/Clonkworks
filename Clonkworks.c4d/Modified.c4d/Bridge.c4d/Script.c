/*-- Brückensegment --*/

#strict
#include BCON

local is_locked;	// Ist im Boden verankert?
local locktype_attach; // is Attached instead of locked?
local PrimaryAttachment; //what object does this depend on while attached?
local locked_perma;

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
  Release(1,,1);
  return(1);
}

protected func RejectEntrance(){
	if(locktype_attach) return(1);
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

private func CollideAll(){
	var x = 0;
	while(x < GetVertexNum()-2){
		SetVertex(x,2,GetVertex(x,2)-64,this(),2);
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
	ObjOnTop = FindObjects( Find_Or(Find_Category(C4D_Vehicle), Find_Category(C4D_Living), Find_Category(C4D_Object)), Find_NoContainer(), Find_OnLine(-30,-7,30,-7), Find_Exclude(this()), Find_Exclude(PrimaryAttachment));
	//Log("Amount of objects on top of %s: %d", GetName(), GetLength(ObjOnTop));
	var bX = GetX();
	var bY = GetY();
	var WasMoved = false;
	//updating velocity and position if needed
	if(!PrimaryAttachment){
		Release(,,1);
		return(0);
	}
	if(PrimaryAttachment && !PrimaryAttachment->~CanAttachBridge()){
		Release(,,1); //release if primary attachment is not supported
		return(0);
	}
	if(GetX() != GetX(PrimaryAttachment)-paX || GetX() != GetY(PrimaryAttachment)-paY){
		SetPosition(GetX(PrimaryAttachment)-paX, GetY(PrimaryAttachment)-paY);
		WasMoved = true;
	}
	if(GetXDir() != GetXDir(PrimaryAttachment)){
		SetXDir(GetXDir(PrimaryAttachment));
		WasMoved = true;
	}
	if(GetYDir() != GetYDir(PrimaryAttachment)){
		SetYDir(GetYDir(PrimaryAttachment));
		WasMoved = true;
	}
	//fixing some of the sliding
	if(GetXDir() > 1) SetXDir(GetXDir() - 1);
	if(GetXDir() < 1) SetXDir(GetXDir() + 1);
	//Setting positions of objects on top.
	var Xdif = GetX()-bX;
	var Ydif = GetY()-bY;
	
	if(WasMoved && GetActTime() > 5){ //act time is here because the bridge does this violent snap when attached because of upright attach.
	for(var i in ObjOnTop){
		if(i->~IsLocked()) continue; //dont drag locked bridges!
		if((GetDefBottom()-GetY())-6 > GetDefBottom(i)-GetY(i)) continue; //dont drag objects that touch the top of the collision line
		SetPosition(GetX(i)+Xdif,GetY(i), i);
	}
	
	for(var j in ObjOnTop){
		if(j->~IsLocked()) continue; //dont drag locked bridges!
		if((GetDefBottom()-GetY())-6 > GetDefBottom(j)-GetY(j)) continue; //dont drag objects that touch the top of the collision line
		SetPosition(GetX(j),GetY(j)+Ydif, j);
	}
	}
	
	//collision detection
	if(GetXDir() < 0 && GBackSolid(-37,0)){
		SetXDir(1);
		DisturbConnections(1, 1, 0);
	}
	
	if(GetXDir() > 0 && GBackSolid(-37,0)){
		SetXDir(1);
		DisturbConnections(1, 1, 0);
	}
	
	if(GetXDir() > 0 && GBackSolid(37,0)){
		SetXDir(-1);
		DisturbConnections(0, -1, 0);
	}
	
	if(GetXDir() < 0 && GBackSolid(37,0)){
		SetXDir(-1);
		DisturbConnections(0, -1, 0);
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

private func Destruction(){
	locked_perma = true;
}

private func Release(quiet, dont_ascend, regardless){
  if(locked_perma && !regardless) return(0);
  if(iAttachedLeft && iAttachedLeft == PrimaryAttachment){
	  PrimaryAttachment = 0;
	  iAttachedLeft->~ClearRight();
	  if(iAttachedRight){
	  iAttachedRight->~ClearLeft();
	  iAttachedRight->~Release();
	  iAttachedRight = 0;
	  }
  }
  
  if(iAttachedRight && iAttachedRight == PrimaryAttachment){
	  PrimaryAttachment = 0;
	  iAttachedRight->~ClearLeft();
	  if(iAttachedLeft){
	  iAttachedLeft->~ClearRight();
	  iAttachedLeft->~Release();
	  iAttachedLeft = 0;
	  }
  }

  if(locktype_attach && !OnFire() && this()){
		locked_perma = true;
		var bg = CreateObject(GetID());
		bg->Release();
		RemoveObject();
		return(1);
  }
  
  SetAction("Idle");
  SetSolidMask();
  is_locked = 0;
  locktype_attach = 0;

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

//unscrew
public func Unscrewable(){ return(IsLocked()); }
public func Unscrew(){
	return(Release());
}

// Nicht an die Dampflok koppeln lassen
public func NoPull()
{
  return IsLocked();
}