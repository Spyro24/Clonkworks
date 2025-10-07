#strict 2

#include CLNK

local ControlDirection;
local NoMoveJump;
local xdr;

func Initialize(){
	SetPlrViewRange(800, this());
	return(_inherited());
}

/* Air Control + WallJumps */
func ControlLeft(){
	if(!GetPlrJumpAndRunControl(GetOwner(this()))) ControlDirection = -1;
	//must be climbing to wall jump
	if(GetAction() == "Scale" || GetAction() == "ScaleDown"){
		//is facing right?
		if(GetDir() == 1){
			//Do The Jump
			Fling(this(), -(100000/(110000-GetPhysical("Jump", 0, 0)))-2,-3);
			SetDir(0);
			SetAction("Jump");
			OnActionJump(GetXDir(0,100), GetYDir(0,100), false);
		}
	}
	
	return(_inherited());
}

func ControlDown(){
	if(!GetPlrJumpAndRunControl(GetOwner(this()))) ControlDirection = 0;
}

func ControlRight(){
	if(!GetPlrJumpAndRunControl(GetOwner(this()))) ControlDirection = 1;
	//must be climbing to wall jump
	if(GetAction() == "Scale" || GetAction() == "ScaleDown"){
		//is facing left?
		if(GetDir() == 0){
			//Do The Jump
			Fling(this(), (100000/(110000-GetPhysical("Jump", 0, 0)))+2,-3);
			SetDir(1);
			SetAction("Jump");
			OnActionJump(GetXDir(0,100), GetYDir(0,100), false);
		}
	}
	
	return(_inherited());
}

protected func ControlUpdate(object self, int comdir, bool dig, bool throw)
{
  ControlDirection = 0;
  if(comdir == 3) ControlDirection = 1;
  if(comdir == 7) ControlDirection = -1;
  return(_inherited(self, comdir, dig, throw));
}

func DoFallControl(){
	xdr = GetXDir();
	//fall control logic
	if(GetAction() == "Jump"){
		if(GetDir() == DIR_Right && ControlDirection == 1 && GetXDir() > 20){
			SetXDir(GetXDir()+ControlDirection*Random(1));
			return(0);
		}
		if(GetDir() == DIR_Left && ControlDirection == -1 && GetXDir() < -20){
			SetXDir(GetXDir()+ControlDirection*Random(1));
			return(0);
		}
		SetXDir(GetXDir()+ControlDirection*RandomX(1,2));
	}
	
	if(GetAction() == "Tumble"){
		SetXDir(GetXDir()+ControlDirection*Random(2));
	}
}

func OnActionJump(int iXDir100, int iYDir100, bool fByCom){
	if(fByCom){
		NoMoveJump = true;
	}else{
		NoMoveJump = false;
	}
	return(0);
}

func DelveJump(){
	if(NoMoveJump){
		if(!GetPlrJumpAndRunControl(GetOwner(this()))) ControlDirection = 0;
		NoMoveJump = false;
		AddEffect("Stopper", this(), 100,1, this());
	}
}

func FxStopperTimer(pTarget, iEffectNumber){
	SetXDir(xdr);
	if(GetPhase() > 0) return(-1);
}

//making the clonk not tumble when hitting a wall

func ContactLeft(){
	if(GetAction() == "Tumble"){
		SetDir(0);
		SetAction("Scale");
	}
}

func ContactRight(){
	if(GetAction() == "Tumble"){
		SetDir(1);
		SetAction("Scale");
	}
}