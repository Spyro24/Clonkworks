/*-- Neues Script --*/

#strict 2
local FullPercent;

public func IsFilled(){ return(FullPercent>=50); }
public func GetOxygen(){ return(FullPercent); }
public func MaxOxygen(){ return(250); }

func Initialize() {
	AddEffect("OxyDesc",this(),20,0,this());
  FullPercent = 250;
  Update();
  return(1);
}

func FxOxyDescInfo(object pTarget, int iEffectNumber){
	var Full = IsFilled();
	if(Full){
		return(Format("$Percentage2$",FullPercent,MaxOxygen()));
	}
	return(Format("$Percentage1$",FullPercent,MaxOxygen()));
}


protected func Update(){
	var Full = IsFilled();
	if(GetEffect("OxygenTank",Contained()) && GetEffect("OxygenTank",Contained(),,4)  == this()){
		SetPicture(0,76,64,64,this());
		return(0);
	}
	
	if(Full){
		SetPicture(64,12,64,64,this());
	}else{
		SetPicture(64,76,64,64,this());
	}
}

public func SetFull(int fullness){
	if(fullness < 0) fullness = 0;
	if(FullPercent == MaxOxygen() && fullness >= MaxOxygen()) return(0);
	if(fullness > MaxOxygen()) fullness = MaxOxygen();
	FullPercent = fullness;
	Update();
	return(1);
}

public func Activate(pClonk){
	[$TxtReplenish$]
	
	
	if(GetBreath(pClonk) == GetPhysical("Breath",,pClonk)/1000){
			Message("$ErrNotInWater$",pClonk);
			pClonk->Sound("CommandFailure*");
			return(1);
	}
	
	if(GetEffect("OxygenTank",pClonk) && GetEffect("OxygenTank",pClonk,,4)  == this()){
		RemoveEffect("OxygenTank",pClonk);
		return(1);
	}
	
	if(GetEffect("OxygenTank",pClonk) && GetEffect("OxygenTank",pClonk,,4) != this()){
		Message("$ErrInUse$",pClonk);
	    pClonk->Sound("CommandFailure*");
	   return(1);
	}
	
	if(!IsFilled()){
		if(ContentsCount(GetID(),Contained()) != 1){
			for(var i in FindObjects(Find_ID(GetID()), Find_Container(Contained()))){
				if(i->~IsFilled()){
					ShiftContents(pClonk);
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
	
	//pClonk->Drink(this());
	AddEffect("OxygenTank",pClonk,120,1,this());
	pClonk->Sound("Breath");
	return(1);
}

//breathe effect
public func FxOxygenTankStart(object pTarget){
	Sound("Fusing",0,pTarget,75,,+1);
}

public func FxOxygenTankTimer(object pTarget, int iEffectNumber, int iEffectTime){
	if(GetOxygen() == 0 || GetBreath(pTarget) == GetPhysical("Breath",,pTarget)/1000 || !GetAlive(pTarget) || Contained() != pTarget){
			return(-1);
	}
	
	DoBreath(1,pTarget);
	SetFull(GetOxygen()-1);
}

public func FxOxygenTankStop(object pTarget, int iEffectNumber, int iReason, bool fTemp){
	Sound("Fusing",0,pTarget,75,,-1);
	Sound("Airlock2");
	Update();
}


func ReplOxy(){
	var Full = IsFilled();
	var wasEmpty = !Full;
	var ctt = ContainedTop();
	if(!ctt) ctt = this();
	
	if(!InLiquid(ContainedTop()) && ctt->GBackSolid() == false){
		if(!Full) SetFull(GetOxygen()+Random(2));
		else if(!Random(3)) SetFull(GetOxygen()+1);
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