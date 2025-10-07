#strict 2

func Initialize() {
  
  return(1);
}

func Attract(){
	var atrc = FindObjects(Find_Func("IsMetal"), Find_Distance(50), Find_Exclude(this()));
	var MaxAtr = 10;
	for(var metl in atrc){
		if(ObjectDistance(metl, this()) <= 10) continue;
		if(GetX(metl) > GetX(this())) SetXDir(GetXDir(metl)-RandomX(3,MaxAtr), metl);
		if(GetX(metl) < GetX(this())) SetXDir(GetXDir(metl)+RandomX(3,MaxAtr), metl);
		if(GetY(metl) > GetY(this())) SetYDir(GetYDir()-MaxAtr, metl);
		if(GetY(metl) < GetY(this())) SetYDir(GetYDir()+MaxAtr, metl);
	}
}

public func Activate(){
	[$Break$]
	  CastParticles("MSpark",5,25,0,0,15,30,RGB(255,223,127),RGB(255,223,127));
	  Sound("AnvilWork*");
	  RemoveObject(this());
}

protected func Hit()
{
  Sound("MetalHit*");
}

func IsMetal(){ return(true); }
func IsAnvilProduct(){ return(1); }