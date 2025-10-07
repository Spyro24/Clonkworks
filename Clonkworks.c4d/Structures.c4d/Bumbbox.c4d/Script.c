/*-- Neues Script --*/

#strict 3
#include HNCB

local DesignatedBumb2;

func Initialize() {
	PollenAmount = 0;
  SetAction("Fill");
  return(1);
}

public func CanBeHarvested(){ return(GetPhase() >= 4); }

public func ControlDig(){
	[$Harvest$|Image=HONY]
	if(!CanBeHarvested()){
		Message("$CantHarvest$",this());
		Sound("CommandFailure1");
		return(0);
	}
			PollenAmount = 0;
			for(var i = 0; i < RandomX(1,GetPhase()); i++){
			var hon = CreateObject(HONY, RandomX(-5,5), RandomX(-5,5));
			}
			Sound("Rip");
}

public func ContextHarvest(object pCaller, bool DontAttemptPath){
	[$Harvest$|Image=HONY|Condition=CanBeHarvested]
	if(ObjectDistance(this(), pCaller) > 15 && !DontAttemptPath){
		SetCommand(pCaller, "MoveTo", this());
		AppendCommand(pCaller, "Call", this(),pCaller,true,,,"ContextHarvest");
		return(0);
	}
	
	if(DontAttemptPath && ObjectDistance(this(), pCaller) > 15)
		return(0);
	
	ControlDig();
}

func Update(){
	if(GetCon() < 100) return(0);
	if(PollenAmount >= 30) PollenAmount = 30;
	SetPhase((PollenAmount / 3) - 1);
	if((PollenAmount / 3) - 1 > RandomX(1,50))
	CreateParticle("HoneyDrip", RandomX(-1,4),RandomX(-2,2), RandomX(-2,2), 0, RandomX(10,25), RGBa(255,255,255));
		
	if(DesignatedBumb && GetAlive(DesignatedBumb)){
	if(DesignatedBumb.Comb && GetID(DesignatedBumb.Comb) == HNCB) {
		DesignatedBumb.Comb.DesignatedBumb = 0;
	}
	DesignatedBumb.Comb = this();
	}
	
	if(DesignatedBumb2 && GetAlive(DesignatedBumb2)){
	if(DesignatedBumb2.Comb && GetID(DesignatedBumb2.Comb) == HNCB) {
		DesignatedBumb2.Comb.DesignatedBumb = 0;
	}
	DesignatedBumb2.Comb = this();
	}
	
	if(DesignatedBumb)
	if(!GetAlive(DesignatedBumb)) DesignatedBumb = nil;;
	if(DesignatedBumb2)
	if(!GetAlive(DesignatedBumb2)) DesignatedBumb2 = nil;;
	
	if(InLiquid()){
		PollenAmount -= 1;
	}
	
	var FoundBumb;
	if(!Random(5))
	if(FoundBumb = FindObject2(Find_ID(BUMB), Find_Distance(400), Find_Exclude(DesignatedBumb), Find_Exclude(DesignatedBumb2), Sort_Random())){
		if(!FoundBumb->IsWild()){
			return(0);
		}
		if(!DesignatedBumb) DesignatedBumb = FoundBumb;
		else if(!DesignatedBumb2) DesignatedBumb2 = FoundBumb;
	};
}
