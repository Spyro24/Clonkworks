/*--- Flint ---*/

#strict

public func ExplodeSize() 
{ 
  return(18); 
}

protected func Hit()
{
  LoamFill(ExplodeSize(),GetX(),GetY(),"Earth-Smooth");
}

func LoamFill(Size,x,y,Mat){
	//for now it just uses the function below, later it will ingore all solid and liquid materials
	DrawMaterialCircle(Mat,x,y,Size,true);
	
	CastParticles("FSpark", 5,50, 0,0, 50, 200, RGBa(179,116,54,177), RGBa(179,116,54,177));
	CastParticles("FSpark", 5,50, 0,0, 50, 200, RGBa(97,59,23,0), RGBa(97,59,23,177));
	Sound("Loambomb*");
	RemoveObject(this());
}

global func DrawMaterialCircle(string szMaterial,int iX,int iY,int iRad,bool sub)
{
for(var x=iX-iRad;x<iX+iRad;++x)
for(var y=iY-iRad;y<iY+iRad;++y)
if((x-iX)**2 + (y-iY)**2 < iRad**2)
DrawMaterialQuad(szMaterial,x,y,x+1,y,x+1,y+1,x,y+1,sub);
return true;
}

/* Kann in der Chemiefabrik hergestellt werden */
public func IsChemicalProduct() { return(1); }