/*-- Neues Script --*/

#strict
local fuel;

func Initialize() {
  fuel = 0;
  SetAction("Walk");
  SetComDir(COMD_Stop);
  return(1);
}

public func ContainedLeft(pByObj){
	if(GetPlrJumpAndRunControl(pByObj->GetController())) return(0);
	SetComDir(COMD_Left);
}

public func ContainedRight(pByObj){
	if(GetPlrJumpAndRunControl(pByObj->GetController())) return(0);
	SetComDir(COMD_Right);
}

public func ContainedDown(pByObj){
	if(GetPlrJumpAndRunControl(pByObj->GetController())) return(0);
	SetComDir(COMD_Stop);
}

public func ContainedDownDouble(pByObj){
	[$Ride$|Image=DSCN]
	if(Abs(GetXDir()) > 5) return(0);
	SetComDir(COMD_Stop);
	Exit(Contents());
	SetAction("Walk");
}

public func ContainedUp(pByObj){
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
		Enter(this(), pByObj);
		SetColorDw(GetColorDw(pByObj));
		SetAction("WalkMount");
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

public func Ejection(){
	if(GetAction() eq "WalkMount") SetAction("Walk");
	if(GetAction() eq "JumpMount") SetAction("Jump");
	
	SetColorDw(GetPlrColorDw(GetOwner()));
}

public func ContainedUpdate(object self, int comdir, bool dig, bool throw)
{
  SetComDir(comdir);

  return(1);
}

public func RejectContents(){ return(true); }

public func UseFuel(){
	if(!Random(6) && fuel > 0 && Abs(GetXDir()) > 15 && (GetAction() eq "Walk" || GetAction() eq "WalkMount")){
		fuel -= 1;
		var sx = 15;
		if(GetDir() == DIR_Left){
			sx = -15;
		}
		
		Smoke(sx, 7, RandomX(5,25));
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

public func DoInfo(){
	if(InLiquid()){
		Incineration();
	}
	
	if(!isBuilt()){
		Exit(Contents());
	}
	
	if(fuel > 0){
		  SetPhysical("Walk", 800000, 2);
	}else{
		Message("{{OBRL}}",this());
		 SetComDir(COMD_Stop);
	}
	
	//hitting the clonk out of the vehicle.
	if(FindObject2(Find_Category(C4D_Object), Find_Distance(20), Find_NoContainer()) && ContentsCount()){
		if(Abs(GetXDir(FindObject2(Find_Category(C4D_Object), Find_Distance(20), Find_NoContainer()))) > 5 || Abs(GetYDir(FindObject2(Find_Category(C4D_Object), Find_Distance(20), Find_NoContainer()))) > 5){
			Exit(Contents());
		}
	}
	
	//no trying to run over monsters kids!
	if(FindObject2(Find_Or(Find_ID(MONS), Find_ID(FMNS)), Find_NoContainer(), Find_AtPoint())){
		if(ContentsCount()){
			var conk = Contents();
			Exit(conk);
			Fling(conk, RandomX(-5,5), -3);
			conk->Sound("Scream");
			SetComDir(COMD_Stop);
			return(0);
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
	return(fuel == 0);
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

public func TryAutoRefuel(pByObj){
	if(GetActionTarget(0, pByObj) == this()) ControlThrow(pByObj);
}

public func ContextRide(pByObj){
	[$Ride$|Image=DSCN]
	if(!CanRide()){
		if(Contents() == pByObj){
			ContainedDownDouble();
		}else{
			Message("$RideFail2$",pByObj);
			pByObj->Sound("CommandFailure1");
			return(0);
		}
	}
	AddCommand(pByObj,"Call",this(),pByObj,,,,"TryRide");
	AddCommand(pByObj,"Grab",this());

}

public func TryRide(pByObj){
	if(GetActionTarget(0, pByObj) == this()) ControlUp(pByObj);
}

public func IsAdvancedProduct(){ return(1); }