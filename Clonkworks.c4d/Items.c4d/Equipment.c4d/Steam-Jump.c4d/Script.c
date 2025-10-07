/*-- Neues Script --*/

#strict 2

local Air;

func Initialize() {
  Chargeup();
}

protected func Chargeup(){
	SetAction("Charged");
	SetPicture(0,74,64,64,this());
}

protected func DischargeAir(SpecialUser){
	if(GetAction() == "Charged"){
	if(!Contained() && !SpecialUser) return 0;
	
	var Targ = Contained();
	if(SpecialUser) Targ = SpecialUser;
	
	if(GetProcedure(Targ) != "FLIGHT") return(0);
	
	SetAction("Charging");
	SetPicture(0,10,64,64,this());
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
	return(0);
}

protected func ChargeAir(){
	if(GBackSky(0,0)) Air+=2;
	else Air += 1;
	
	if(Air > (38 * 120)){
		Air = 0;
		Sound("Lever1");
		Chargeup();
	}
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