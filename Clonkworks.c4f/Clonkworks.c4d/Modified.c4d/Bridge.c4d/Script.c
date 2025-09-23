/*-- Brückensegment --*/

#strict
#include BCON

local is_locked;	// Ist im Boden verankert?
local locktype_attach; // is Attached instead of locked?

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
	Attachable = FindObject2(Find_Category(C4D_Vehicle), Find_AtPoint(0,0), Find_Func("CanAttachBridge"));
	if(!Attachable) Attachable = FindObject2(Find_Category(C4D_Vehicle), Find_AtPoint(0,10), Find_Func("CanAttachBridge")); //not on exact position? why not check the bottom?
	if(Attachable){ //finding and setting the attachable object
		//left or right?
		if(GetX() <= GetX(Attachable) && Attachable->CanAttachLeft()){
			is_locked = 1;
			locktype_attach = 1;
			LocalN("iAttachedLeft",Attachable) = this();
			iAttachedRight = Attachable;
			SetAction("Attached", Attachable);
			SetActionData(256*RightVert() + Attachable->LeftVert(), this());
			SetSolidMask(0, 11, 72, 3);
			Sound("Connect");
			return(1);
		}else if(GetX() > GetX(Attachable) && Attachable->CanAttachRight()){
			is_locked = 1;
			locktype_attach = 1;
			LocalN("iAttachedRight",Attachable) = this();
			iAttachedLeft = Attachable;
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
	  iAttachedLeft->ClearRight();
	  iAttachedLeft->~Release();
	  iAttachedLeft = 0;
  }
   if(iAttachedRight){
	  iAttachedRight->ClearLeft();
	  iAttachedRight->~Release();
	  iAttachedRight = 0;
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

// Nicht an die Dampflok koppeln lassen
public func NoPull()
{
  return IsLocked();
}
