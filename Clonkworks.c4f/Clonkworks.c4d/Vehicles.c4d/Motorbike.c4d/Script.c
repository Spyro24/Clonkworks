/*-- Neues Script --*/

#strict
local fuel;
local GhostRider;

public func SetGhostRider(val){
	GhostRider = val;
}

func Initialize() {
  SetPhysical("Walk", 700000, 2);
  fuel = 0;
  SetAction("Walk");
  SetComDir(COMD_Stop);
  return(1);
}

public func ContainedLeft(pByObj){
	if(fuel == 0){
		Sound("StartFail");
		return(0);
	}
	if(GetPlrJumpAndRunControl(pByObj->GetController())) return(0);
	if(GetCommand()){
		FinishCommand();
	}
	SetComDir(COMD_Left);
}

public func ContainedRight(pByObj){
	if(fuel == 0){
		Sound("StartFail");
		return(0);
	}
	if(GetPlrJumpAndRunControl(pByObj->GetController())) return(0);
	if(GetCommand()){
		FinishCommand();
	}
	SetComDir(COMD_Right);
}

public func ContainedDown(pByObj){
	if(GetPlrJumpAndRunControl(pByObj->GetController())) return(1);
	FinishCommand();
	SetComDir(COMD_Stop);
	return(1);
}

public func ContainedDownDouble(pByObj){
	[$Ride$|Image=DSCN]
	if(Abs(GetXDir()) > 5) return(1);
	SetComDir(COMD_Stop);
	Exit(Contents());
	Sound("Click");
	SetAction("Walk");
	return(1);
}

public func ContainedUp(pByObj){
		if(fuel == 0){
		Sound("StartFail");
		return(0);
	}
	if(GetCommand()){
		FinishCommand();
	}
	var dr = GetXDir();
	if(Jump()){
	SetXDir(dr);
	if(GetAction() eq "Jump" && ContentsCount()) SetAction("JumpMount");
	}
}

public func ControlUp(pByObj){
	[$Ride$|Image=DSCN]
	if(!isBuilt()) return(0);
	if(Hostile(GetOwner(), GetOwner(pByObj))){
		Message("$RideFail1$",pByObj);
		pByObj->Sound("CommandFailure1");
		return(0);
	}
	if(!ContentsCount()){
		Sound("Click");
		Enter(this(), pByObj);
		SetColorDw(GetColorDw(pByObj));
		SetAction("WalkMount");
	    SetCommand(pByObj,"Grab",this());
		
		// making other objects ungrab
		var grabbers = FindObjects(Find_Action("Push"), Find_ActionTarget(this()), Find_Exclude(Contents()));
		for(var i in grabbers){
			i->SetAction("Walk");
		}
	}
}

public func ControlThrow(pByObj){
	[$Refuel$|Image=OBRL]
	if(fuel != 0){
		Message("$FuelFail1$",pByObj);
		pByObj->Sound("CommandFailure1");
		return(0);
	}
	
	if(ContentsCount(OBRL, pByObj) == 0){
		Message("$FuelFail2$",pByObj);
		pByObj->Sound("CommandFailure1");
		return(0);
	}
	
	var Barrel = FindObject2(Find_ID(OBRL), Find_Container(pByObj));
	if(Barrel){
		ChangeDef(BARL, Barrel);
		fuel = 200;
		Sound("Splash2");
	}
}

protected func Ejection(){
	if(GetAction() eq "WalkMount") SetAction("Walk");
	if(GetAction() eq "JumpMount") SetAction("Jump");
	
	SetColorDw(GetPlrColorDw(GetOwner()));
}

public func ContainedUpdate(object self, int comdir, bool dig, bool throw)
{
  FinishCommand();
  SetComDir(comdir);

  return(1);
}

public func RejectContents(){ return(true); }

protected func UseFuel(){
	if(!Random(8) && fuel > 0 && Abs(GetXDir()) > 15 && (GetAction() eq "Walk" || GetAction() eq "WalkMount")){
		fuel -= 1;
		var sx = 15;
		if(GetDir() == DIR_Left){
			sx = -15;
		}
		
		Smoke(sx, 7, RandomX(5,25));
		
		if(GetCursor(GetOwner(Contents()), 0) == Contents()){
		if(fuel == 100){
			PlayerMessage(GetOwner(Contents()), "$FuelWarn50$");
		}
		
		if(fuel == 50){
			PlayerMessage(GetOwner(Contents()), "$FuelWarn25$");
		}
		
		if(fuel == 20){
			PlayerMessage(GetOwner(Contents()), "$FuelWarn10$");
		}
		}
		
		if(fuel == 0){
			Sound("Discharge");
		}
	}
}

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 if (szCommand eq "MoveTo" || szCommand eq "Dig")
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(0);
}

protected func DoInfo(){
	if(InLiquid()){
		Incineration();
	}
	
	if(!Contents() && !GhostRider && GetCommand() ne "Exit"){
		FinishCommand();
	}
	
	if(!isBuilt()){
		Exit(Contents());
	}
	
	if(fuel <= 0){
		if(!Contained())
		Message("{{OBRL}}",this());
		 SetComDir(COMD_Stop);
	}
	
	if (GetContact(this(), -1) & CNAT_Bottom){
		SetR(0);
	}
	
	//hitting the clonk out of the vehicle.
	if(FindObject2(Find_Category(C4D_Object), Find_Distance(20), Find_NoContainer()) && ContentsCount()){
		if(Abs(GetXDir(FindObject2(Find_Category(C4D_Object), Find_Distance(20), Find_NoContainer()))) > 5 || Abs(GetYDir(FindObject2(Find_Category(C4D_Object), Find_Distance(20), Find_NoContainer()))) > 5){
			Exit(Contents());
		}
	}
	
	//no trying to run over monsters kids!
	if(FindObject2(Find_Or(Find_ID(MONS), Find_ID(FMNS)), Find_NoContainer(), Find_AtPoint())){
		var mons = FindObject2(Find_Or(Find_ID(MONS), Find_ID(FMNS)), Find_NoContainer(), Find_AtPoint());
		if(ContentsCount() && !Random(20) && GetSpeed() < 160){
			var conk = Contents();
			Exit(conk);
			Punch(conk,RandomX(5,12));
			Fling(conk, RandomX(-5,5), -3);
			conk->Sound("Scream");
			Sound("ClonkHit*");
			SetComDir(COMD_Stop);
			return(0);
		}
	}
	if(FindObjects(Find_NoContainer(), Find_AtPoint(), Find_OCF(OCF_Alive)) && DangerousMode() && GetSpeed() >= 160 ){
		var list = FindObjects(Find_NoContainer(), Find_AtPoint(), Find_OCF(OCF_Alive));
		for(var mons in list){
		if(GetSpeed() > 160 && GetX(mons) < GetX() && DangerousMode()){ //crashing to the left, less sensetive than into a wall.
			if(GetCreatureSize(mons) > 30) ContactLeft();
			Punch(mons,RandomX(10,35));
			Fling(mons, -RandomX(7,14), -4);
		}else if(GetSpeed() > 160 && GetX(mons) > GetX() && DangerousMode()){ //crashing to the right, less sensetive than into a wall.
			if(GetCreatureSize(mons) > 30) ContactRight();
			Punch(mons,RandomX(10,35));
			Fling(mons, RandomX(7,14), -4);
		}
		}
	}
	
	if(GetDir() == DIR_Left){
	if(GetYDir() > 0){
		SetRDir(-2);
	}else if(GetYDir() < 0){
		SetRDir(1);
	}
	}else if(GetDir() == DIR_Right){
	if(GetYDir() > 0){
		SetRDir(1);
	}else if(GetYDir() < 0){
		SetRDir(-2);
	}
	}
	
	if(Abs(GetXDir()) > 15 && (GetAction() eq "Walk" || GetAction() eq "WalkMount")){
		Sound("MotoDrive",0,this(),100,0,+1);
	}else{
		Sound("MotoDrive",0,this(),100,0,-1);
	}
}

private func Incineration(){
	Exit(Contents());
	return(Explode(RandomX(25,35)));
}

public func isBuilt(){
	return(GetCon() >= 100);
}

protected func CheckWalk(){
	if(ContentsCount()){
		SetAction("WalkMount");
	}
	
	return(1);
}

protected func CheckJump(){
	if(ContentsCount()){
		SetAction("JumpMount");
	}
	
	return(1);
}

protected func CanRefuel(){
	if(!CanRide()) return(0);
	return(fuel <= 100);
}

protected func CanRide(){
	return(!ContentsCount());
}

public func ContextRefuel(pByObj){
	[$Refuel$|Image=OBRL|Condition=CanRefuel]
		AddCommand(pByObj,"Call",this(),pByObj,,,,"TryAutoRefuel");
			AddCommand(pByObj,"Grab",this());
	AddCommand(pByObj,"Acquire",,,,,,OBRL);
}

protected func TryAutoRefuel(pByObj){
	if(GetActionTarget(0, pByObj) == this()) ControlThrow(pByObj);
}

public func ContextRide(pByObj){
	[$Ride$|Image=DSCN]
	if(!CanRide()){
		if(Contents() == pByObj){
			ContainedDownDouble();
			return(1);
		}else{
			Message("$RideFail2$",pByObj);
			pByObj->Sound("CommandFailure1");
			return(0);
		}
	}
	AddCommand(pByObj,"Call",this(),pByObj,,,,"TryRide");
	AddCommand(pByObj,"Grab",this());

}

protected func TryRide(pByObj){
	if(GetActionTarget(0, pByObj) == this()) ControlUp(pByObj);
}

public func IsAdvancedProduct(){ return(1); }

//Crash logic

protected func ContactTop(){
	if(GetR() >= 100 && GetR() <= 265){
		Sound("ClonkHit*");
		Exit(Contents());
		SetR(0);
	}
}

protected func ContactLeft(){
	if(GetSpeed() > 160){
			if(GetR() >= 100 && GetR() <= 265){
				ContactTop();
				return(1);
			}	
			Sound("ClonkHit*");
			if(ContentsCount()){
			var conk = Contents();
			Exit(conk);
			if(!DangerousMode()){
				Punch(conk,RandomX(15,33));
			}
			Fling(conk, RandomX(5,10), -4);
			conk->Sound("Scream");
			}
			SetComDir(COMD_Stop);
			if(!Random(12)) Incineration();
			Fling(this(), RandomX(5,10), -4);
			return(1);
	}
}

protected func ContactRight(){
	if(GetSpeed() > 160){
		if(GetR() >= 100 && GetR() <= 265){
				ContactTop();
				return(1);
			}	
			Sound("ClonkHit*");
			if(ContentsCount()){
			var conk = Contents();
			Exit(conk);
			if(!DangerousMode()){
				Punch(conk,RandomX(15,33));
			}
			Fling(conk, -RandomX(5,10), -4);
			conk->Sound("Scream");
			}
			SetComDir(COMD_Stop);
			if(!Random(12)) Incineration();
			Fling(this(), -RandomX(5,10), -4);
			return(1);
	}
}

//other logic
protected func RejectEntrance(pIntoObj){
	if(Contents()) return(true);
}

protected func RejectGrabbed(){
	if(Contents()) return(true);
}

public func DangerousMode(){
	return(ObjectCount(RDKL) > 0);
}

public func GetCreatureSize(Creature){
	return(Max(GetDefCoreVal("Width", "DefCore", GetID(Creature)), GetDefCoreVal("Height", "DefCore", GetID(Creature))));
}