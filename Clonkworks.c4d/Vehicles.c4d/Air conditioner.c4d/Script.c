/*-- Neues Script --*/

#strict 2
#include CXEC

func Initialize() {
  SetAction("Idle");
  return(1);
}

public func ControlThrow(pByObj){
	[$TxtLeft$]
	if(GetEnergy() == 0 && FindObject(ENRG)) return(1);
	if(GetAction() == "Heating") return(1);
	Sound("Click");
	if(GetAction() == "Idle") return(SetAction("Heating"));
	if(GetAction() == "Cooling") return(SetAction("Idle"));
}
public func ControlDig(pByObj){
	[$TxtRight$]
	if(GetEnergy() == 0 && FindObject(ENRG)) return(1);
	if(GetAction() == "Cooling") return(1);
	Sound("Click");
	if(GetAction() == "Idle") return(SetAction("Cooling"));
	if(GetAction() == "Heating") return(SetAction("Idle"));
}

//heating and cooling effects
public func DoHeat(){
	if(InLiquid()){
		Sound("Discharge");
		SetAction("Idle");
		return(0);
	}
	
	CreateParticle("PSpark",RandomX(-8,8),RandomX(-5,5)-5,0,0,RandomX(25,40),RGBa(255,140,0),this());
	if(!Random(5)) Smoke(0,-5,RandomX(5,15));
	//finding a random pixel nearby and heating it up!
	for(var i = 0; i < Random(200); i++){
	var iX, iY, range;
	if(Contained()) range/=2;
	iX = GetX() + RandomX(-range,range);
	iY = GetY() + RandomX(-range,range);
	
	var mat = GetMaterial(iX-GetX(),iY-GetY());
	var HeatedMat, MatTex;
	HeatedMat = GetMaterialVal("AboveTempConvertTo", "Material", mat);
	if(GetMaterialVal("AboveTempConvert", "Material", mat) > TempRange()) continue;
	if(!HeatedMat || HeatedMat == "") continue;
	MatTex = GetMaterialVal("TextureOverlay", "Material", Material(HeatedMat));
	
	var Underground = UndergroundXY(iX,iY);
	if(MatTex){
		CreateMatPx(Format("%s-%s",HeatedMat,MatTex),iX,iY,Underground);
	}else{
		CreateMatPx(Format("%s",HeatedMat),iX,iY,Underground);
	}
	}
}

public func DoCooling(){
	if(InLiquid()){
		Sound("Discharge");
		SetAction("Idle");
		return(0);
	}
	
	CreateParticle("PSpark",RandomX(-8,8),RandomX(-5,5)-5,0,0,RandomX(25,40),RGBa(0,140,255),this());
	//finding a random pixel nearby and cooling it off!
	for(var i = 0; i < Random(200); i++){
	var iX, iY, range;
	range = 67;
	iX = GetX() + RandomX(-range,range);
	iY = GetY() + RandomX(-range,range);
	
	var mat = GetMaterial(iX-GetX(),iY-GetY());
	var HeatedMat, MatTex;
	HeatedMat = GetMaterialVal("BelowTempConvertTo", "Material", mat);
	if(GetMaterialVal("BelowTempConvert", "Material", mat) < -TempRange()) continue;
	if(!HeatedMat || HeatedMat == "") continue;
	MatTex = GetMaterialVal("TextureOverlay", "Material", Material(HeatedMat));
	
	var Underground = UndergroundXY(iX-GetX(),iY-GetY());
	
	if(MatTex){
		CreateMatPx(Format("%s-%s",HeatedMat,MatTex),iX,iY,Underground);
	}else{
		CreateMatPx(Format("%s",HeatedMat),iX,iY,Underground);
	}
	}
}

public func TempRange(){ return(15); }

//basic stuff
public func IsAdvancedProduct(){ return(1); }

//what advanced lines can be connected?
public func ALKConnectType(){
	return([TRPW]);
}

//global funcs
global func UndergroundXY(int x, int y){
	var iX = AbsX(x);
	var iY = AbsY(y);
	var iMat = GetMaterial(iX,iY);
	var iTex = GetMaterialVal("TextureOverlay", "Material", iMat);
	//remove pixel here 20 times just in case.
	for(var i = 0; i < 20; i++){
	BlastFree(iX,iY,1);
	}
	
	if(GetMaterial(iX,iY) != Material("Sky")){
		if(iTex){
			CreateMatPx(Format("%s-%s",MaterialName(iMat),iTex),x,y,1);
		}else{
			CreateMatPx(Format("%s",MaterialName(iMat)),x,y,1);
		}
		return(1);
	}
	
	if(iTex){
			CreateMatPx(Format("%s-%s",MaterialName(iMat),iTex),x,y,0);
	}else{
			CreateMatPx(Format("%s",MaterialName(iMat)),x,y,0);
	}
	return(0);
}

//damage
public func Damage(int iChange, int iByPlayer){
	if(GetDamage() > 55) Explode(30);
}