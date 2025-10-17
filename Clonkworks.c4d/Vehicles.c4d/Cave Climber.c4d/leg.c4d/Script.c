/*-- Neues Script --*/

#strict 2

local tX;
local tY;
local ttX;
local ttY;
local vert;

public func GetLegVert(){
	return(vert);
}

public func SetLegVert(v){
	vert = v;
}

func Initialize() {
  
  return(1);
}

public func MoveToPlace(iX,iY){
	if(GetEffect("Legmove",this())) return(0);
	tX = iX;
	tY = iY;
	ttX = GetX()+tX;
	ttY = GetY()+tY;
	
	var XDif = AbsX(GetX()+tX);
	XDif/=16;
	var YDif = AbsY(GetY()+tY);
	YDif/=16;
	
	tX = XDif;
	tY = YDif;
	
	SetAction("MoveLeg");
	AddEffect("Legmove",this(),100,1,this());
	return(1);
}

protected func FxLegmoveTimer(object pTarget, int iEffectNumber, int iEffectTime){
	if(iEffectTime > 16) return(-1);
	if(!GetBGWall(tX,tY)) return(-1);
	SetPosition(GetX()+tX,GetY()+tY);
}

protected func FxLegmoveStop(){
	SetAction("Idle");
}

public func IsAttachedToWall(){
	if(GetAction() == "MoveLeg"){
		return(0);
	}
	//return(GetBGWall(GetVertex(0,0),GetVertex(0,1)) || GBackSolid(GetVertex(0,0), GetVertex(0,1)));
	return(GetBGWall() || GBackSolid());
}

protected func Clonk(){
		var Mat = GetMaterial(GetVertex(0,0),GetVertex(0,1));
		if (Mat == -1) return(0);
		var col1,col2;
		col1 = RGBa(GetMaterialVal("Color", "Material", Mat,0),GetMaterialVal("Color", "Material", Mat,1),GetMaterialVal("Color", "Material", Mat,2));
		if(GetMaterialVal("Color", "Material", Mat,3)) col2 = RGBa(GetMaterialVal("Color", "Material", Mat,3),GetMaterialVal("Color", "Material", Mat,4),GetMaterialVal("Color", "Material", Mat,5));
		else col2 = col1;
		
		col1 = RGBa(BoundBy(GetRGBaValue(col1,1)-50,0,255), BoundBy(GetRGBaValue(col1,2)-50,0,255), BoundBy(GetRGBaValue(col1,3)-50,0,255));
		col2 = RGBa(BoundBy(GetRGBaValue(col2,1)-50,0,255), BoundBy(GetRGBaValue(col2,2)-50,0,255), BoundBy(GetRGBaValue(col2,3)-50,0,255));
		
		CastParticles("PxSpark",RandomX(3,9),25,GetVertex(0,0),GetVertex(0,1),10,40,col1,col2);
		
		Sound("ClimbHit*");
}