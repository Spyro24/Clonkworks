/*-- Neues Script --*/

#strict

local WindHeld;

func Initialize() {
  SetAction("OnGround");
  return(1);
}

//Packing the glider
public func ControlDigDouble(pByObject){
	[Pack|Image=PHNG]
	if(GetAction() ne "OnGround" && GetAction() ne "Hit") return(0);
	var kit = CreateObject(PHNG,0,0,GetOwner());
	if(pByObject) Enter(pByObject,kit);
	Sound("Click");
	RemoveObject();
	return(1);
}

func CheckGround(){
	//check if its on the ground, if not - liftoff!
	if(CanGlide() && GetYDir()>10){
		WindHeld = GetWind(0,0);
		SetAction("Fly");
		return(1);
	}
	return(1);
}

func FlightLogic(){
	//flying logic
/* 	if(!CanGlide()){
		SetAction("OnGround");
		return(0);
	} */
	
	if(GetWind(0,0) > WindHeld) WindHeld++;
	if(GetWind(0,0) < WindHeld) WindHeld--;
	
	var downwardsLimit;
	if(Abs(WindHeld) > 90) downwardsLimit = 4;
	else if(Abs(WindHeld) > 60) downwardsLimit = 6;
	else if(Abs(WindHeld) > 30) downwardsLimit = 8;
	else if(Abs(WindHeld) > 5) downwardsLimit = 12;
	else downwardsLimit = 9999;
	
	if(GetYDir() > downwardsLimit) SetYDir(downwardsLimit);
	
	if(Abs(GetXDir()) < Abs(WindHeld/3))
	if(GetWind(0,0) < 0){
		SetXDir(GetXDir()-1);
	}else{
		SetXDir(GetXDir()+1);
	};
	
	if(!Random(2))
	if(WindHeld < 0 && GetXDir() > 0) SetXDir(GetXDir()-1);;
	if(!Random(2))
	if(WindHeld > 0 && GetXDir() < 0) SetXDir(GetXDir()+1);;
	
	if(GetXDir() < 0) SetDir(0);
	if(GetXDir() > 0) SetDir(1);
}

func ContactLeft(){
	if(GetAction() S= "Fly" || GetAction() S= "FlyTurn"){
	if(GetDir() == DIR_Right) return(0); //you cant hit a wall to the right when youre going left!
	ShakeOffRopes();
	SetXDir(-(WindHeld/2));
	SetYDir(-15);
	SetAction("Hit");
	Sound("ArrowHit");
	}
}

func ContactRight(){
	if(GetAction() S= "Fly" || GetAction() S= "FlyTurn"){
	if(GetDir() == DIR_Left) return(0); //you cant hit a wall to the left when youre going right!
	ShakeOffRopes();
	SetXDir(WindHeld/2);
	SetYDir(-15);
	SetAction("Hit");
	Sound("ArrowHit");
	}
}

func ContactBottom(){
	if(GetAction() S= "Fly" || GetAction() S= "FlyTurn"){
		ShakeOffRopes();
	}
	if(GetAction() S= "Hit") return(0);
	SetAction("OnGround");
}

func ShakeOffRopes(){
	while(FindRope(this(), 0) != 0){
		RemoveObject(FindRope(this(), 0));
		CreateObject(CK5P);
	}
}

public func Poke()
{
  Sound("Rip");
  SetAction("Hit");
}

protected func CanGlide(){
	if(GetContact(this(), -1) & CNAT_Bottom) return(0); // no touching the ground
		if(GBackSky(0,0)) //is in the sky?
			if(Abs(GetWind(0,0)) > 25) //is wind good?
				if(!Contained()) // not contained
				return(1);
	return(0);
}