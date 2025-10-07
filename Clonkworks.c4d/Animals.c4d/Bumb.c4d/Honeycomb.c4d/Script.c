/*-- Neues Script --*/

#strict

local PollenAmount;
local DesignatedBumb;

func Initialize() {
  PollenAmount = 0;
  SetAction("Fill");
  return(1);
}

func Update(){
	if(PollenAmount >= 30) PollenAmount = 30;
	SetPhase((PollenAmount / 3) - 1);
	if((PollenAmount / 3) - 1 > RandomX(1,50))
	CreateParticle("HoneyDrip", RandomX(-10,10),RandomX(-10,10), RandomX(-2,2), 0, RandomX(10,25), RGBa(255,255,255));

	if(GetMaterial(0,0) != Material("Tunnel")){
		Sound("Dig");
		RemoveObject();
	}

	if(!DesignatedBumb){
		DoCon(-1);
	}else{
		SetCon(100);
	}
	
	if(PollenAmount == 30 && !Random(30)){
			PollenAmount = 0;
			for(var i = 0; i < RandomX(1,10); i++){
			var hon = CreateObject(HONY, RandomX(-5,5), RandomX(-5,5));
			LocalN("Wild",hon) = true;
			SetXDir(RandomX(-10,10), hon);
			SetYDir(RandomX(-10,10), hon);
			}
			Sound("splat*");
		}
		
		
	if(InLiquid()){
		PollenAmount -= 1;
	}
}
