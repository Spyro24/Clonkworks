#strict 2

local MaterialCheck;

func Initialize() {
  MaterialCheck = [METL,GOLD,CRYS,CPIG,TTIG,ORE1,ORE2,ORE3]; //materials that can be attracted
  return(1);
}

func Attract(){
	if(Contained()){
		if( !(GetCategory(ContainedTop(this())) & C4D_Vehicle) && !(GetCategory(ContainedTop(this())) & C4D_Living) ){
			return(0);
		}
	}
	var atrc = FindObjects(Find_Category(C4D_Object), Find_Distance(50), Find_Exclude(this()));
	var MaxAtr = 10;
	for(var metl in atrc){
		if(ObjectDistance(metl, this()) <= 10) continue;
		var canAttract = false;
		for(var mt in MaterialCheck){
			if(GetComponent(mt,,metl) != 0 || GetID(metl) == mt) canAttract = true;
		}
		if(!canAttract) continue;
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