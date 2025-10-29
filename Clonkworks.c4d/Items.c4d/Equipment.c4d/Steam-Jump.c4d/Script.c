/*-- Neues Script --*/

#strict 2

local Air;

func Initialize() {
  Chargeup();
  AddEffect("AirStatus",this(),1,0,this());
}

protected func Chargeup(){
	SetAction("Charged");
	SetPicture(0,75,64,64,this());
}

//status effect
protected func FxAirStatusInfo(object pTarget, int iEffectNumber){
	if(GetAction() == "Charged") return("$Status1$");
	if(GetAction() == "Charging") return(Format("$Status2$",30-(Air/38)));
	return("$Status3$");
}

protected func DischargeAir(SpecialUser){
	if(GetAction() == "Charged"){
	if(!Contained() && !SpecialUser) return 0;
	
	var Targ = Contained();
	if(SpecialUser) Targ = SpecialUser;
	
	if(GetProcedure(Targ) != "FLIGHT") return(0);
	
	SetAction("Idle");
	SetPicture(0,11,64,64,this());
	Sound("SteamBlast*");
	
	var vX =GetXDir(Targ);
	var vY =GetYDir(Targ);
	
	var nvX = vX * 3;
	var nvY = vY * 3;
	
	if(Abs(vX) < 50)
	if(nvX > 0)
	nvX = Min(50, nvX);
	else
	nvX = Max(-50, nvX);;

	if(Abs(vY) < 50)
	if(nvY > 0)
	nvY = Min(50, nvY);
	else
	nvY = Max(-50, nvY);;

	if(vX > 50) nvX = vX + 50;
	if(vX < -50) nvX = vX - 50;
	if(vY > 50) nvY = vY + 50;
	if(vY < -50) nvY = vY - 50;
	
	SetXDir(nvX, Targ);
	SetYDir(nvY, Targ);
	//Contained()->SetAction("Tumble");
	
	for(var i = 0; i < RandomX(10,25); i++){
		CreateParticle("PSpark", 0,0,RandomX(-vX*2,vX/2),RandomX(-vY*2,0),RandomX(50,170),RGBa(255,255,255,RandomX(0,10)));
	}
	
	return(1);
	}
	
	if(GetAction() == "Idle") SetAction("Charging");
	else SetAction("Idle");
	
	Sound("QuickClose");
	
	return(1);
}

protected func ChargeAir(){
	if(Stuck() || InLiquid()){
		SetAction("Idle");
		Sound("QuickClose");
		return(0);
	}
	Air++;
	
	if(Air > (38 * 30)){
		Air = 0;
		Sound("JarFill");
		Chargeup();
	}
	
	var pX, pY;
	pX = RandomX(-1,1);
	pY = RandomX(-1,1);
	if(!pX || !pY) return(0);
	pX *= RandomX(0,15);
	pY *= RandomX(0,15);
	
	var pXDir, pYDir, pAngle;
	pAngle=Angle(GetX(),GetY(),GetX()+pX,GetY()+pY);
	
	pXDir = Sin(pAngle+180, 5);
	pYDir = -Cos(pAngle+180, 5);
	if(GetCategory(ContainedTop()) & C4D_Structure) return(0);
	if(GetCategory(ContainedTop()) & C4D_Vehicle) return(0);
	var top = ContainedTop();
	if(!top) top = this();
	CreateParticle("PSpark",pX,pY,pXDir,pYDir,RandomX(20,35),RGBa(255,255,255,RandomX(0,25)),top);
}

public func Activate(){
	[$Launch$]
	return DischargeAir();
}

public func Departure(thrower){
	if(GetAction() == "Charging" || GetProcedure(thrower) != "FLIGHT") return(0);
	DischargeAir(thrower);
	Enter(thrower);
}

public func Hit(){
	if(!Random(3)) Sound("MetalHit*");
}

public func IsAnvilProduct(){ return(true); }
public func IsAdvancedProduct(){ return(true); }
public func IsExtraItem(){ return(true); }
public func IsMetal(){ return(true); }