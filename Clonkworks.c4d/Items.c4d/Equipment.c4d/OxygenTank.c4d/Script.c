/*-- Neues Script --*/

#strict
local Full;
local FullPercent;

public func IsFilled(){ return(Full); }
public func GetOxygen(){ return(FullPercent); }

func Initialize() {
	AddEffect("OxyDesc",this(),20,0,this());
  Full = true;
  FullPercent = 100;
  Update();
  return(1);
}

func FxOxyDescInfo(object pTarget, int iEffectNumber){
	if(Full){
		return(Format("$Percentage2$",FullPercent));
	}
	return(Format("$Percentage1$",FullPercent));
}


protected func Update(){
	Full = (FullPercent >= 50);
	if(Full){
		SetPicture(64,12,64,64,this());
	}else{
		SetPicture(64,76,64,64,this());
	}
}

public func SetFull(int fullness){
	if(fullness < 0) fullness = 0;
	if(FullPercent == 100 && fullness >= 100) return(0);
	if(fullness > 100) fullness = 100;
	FullPercent = fullness;
	Update();
	return(1);
}

public func Activate(pClonk){
	[$TxtReplenish$]
	
	if(!InLiquid(pClonk)){
			Message("$ErrNotInWater$",pClonk);
			pClonk->Sound("CommandFailure*");
			return(1);
	}
	
	if(!Full){
		if(ContentsCount(GetID(),Contained()) != 1){
			for(var i in FindObjects(Find_ID(GetID()), Find_Container(Contained()))){
				if(i->~IsFilled()){
					i->Activate(pClonk);
					return(1);
				}
			}
			
			Message("$ErrNoBreathable$",pClonk);
			pClonk->Sound("CommandFailure*");
			return(1);
		}else{
			Message("$ErrNoBreathableSolo$",pClonk);
			pClonk->Sound("CommandFailure*");
			return(1);
		}
	}
	
	pClonk->Drink(this());
	DoBreath(GetPhysical("Breath", 0, GetCursor(0))*FullPercent/100000, pClonk);
	SetFull(0);
	pClonk->Sound("Breath");
	return(1);
}

func ReplOxy(){
	var wasEmpty = !Full;
	if(!InLiquid(ContainedTop())){
		if(!Full) SetFull(GetOxygen()+Random(2));
		else if(!Random(7)) SetFull(GetOxygen()+1);
	}

	if(Full && wasEmpty) Sound("Airlock1");
	
	if(Full && !Contained() && InLiquid()){
		if(!Random(2)) Bubble();
	}
}

func Hit(){
	if(!Random(3)) Sound("ClonkHit*");
}

public func IsAdvancedProduct(){ return(1); }
public func IsAnvilProduct(){ return(1); }