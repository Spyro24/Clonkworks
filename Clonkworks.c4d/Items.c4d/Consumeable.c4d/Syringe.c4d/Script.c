/*-- Neues Script --*/

#strict 2
local Essence;

func Initialize() {
  Update();
  AddEffect("Syrinf",this(),1,0,this());
  return(1);
}

func Update(){
	if(GetAction() != "Filled"){
		SetPicture(17,22,33,41);
	}else{
		SetPicture(11,85,40,57);
	}
}

public func Activate(pClonk){
	[$TxtInject$]
	if(!Essence) SetAction("Idle");
	var NotFilled = true;
	if(GetAction() == "Filled") NotFilled = false;
	
	if(GetEffect("Injection",pClonk)) return(1);
	
	if(NotFilled){
	var Injectable = FindObject2(Find_Func("HasEssence"), Find_AtPoint(), Find_NoContainer());
	if(!Injectable){
		Message("$ErrNoCorpse$",Contained());
		Contained()->Sound("CommandFailure1");
		return(1);
	}
	
	AddEffect("Injection",pClonk,300,1,this());
	}
	
	if(!NotFilled){
		if(GetAction(pClonk) != "Walk") {
			Message("$ErrBadPos$",Contained(),GetName(Contained()));
			Contained()->Sound("CommandFailure1");
			return(1);
		}
		
		if(GetEffect("Injected",pClonk)){
			Message("$ErrAlreadyUsed$",Contained());
			Contained()->Sound("CommandFailure1");
			return(1);
		}
		
		AddEffect("Injection",pClonk,300,1,this());
	}
	
	return(1);
}

//Injection effect! comes with cool animation :D
protected func FxInjectionStart(pTarget,iEffectNumber,iTemp){
	//make the clonk kneel down to inject.
	if(GetActMapVal("Name","KneelDown",GetID(pTarget)))
	pTarget->SetAction("KneelDown");
}

protected func FxInjectionTimer(pTarget,iEffectNumber,iEffectTime){
	//not kneeling? lets see how to end this...
	if(GetAction(pTarget) != "KneelDown"){
		//if the target has a kneeling animation
		if(GetActMapVal("Name","KneelDown",GetID(pTarget)) && GetActMapVal("Name","KneelUp",GetID(pTarget))){
			if(GetAction(pTarget) == "KneelUp") EffectVar(0,pTarget,iEffectNumber) = 1; //ok, can do injection!
			return(-1);
		}else{
			if(GetAction(pTarget) == "Walk" || GetAction(pTarget) == "Idle") EffectVar(0,pTarget,iEffectNumber) = 1; //ok, can do injection!
			return(-1);
		}
	}
}

protected func FxInjectionStop(pTarget,iEffectNumber,iReason,fTemp){
	if(EffectVar(0,pTarget,iEffectNumber) == 1) Inject();
}


protected func Inject(){
	var NotFilled = true;
	if(GetAction() == "Filled") NotFilled = false;
	
	if(NotFilled) DoExtraction();
	else DoInjection();
}

public func DoExtraction(){
	var Injectable = FindObject2(Find_Func("HasEssence"), Find_AtPoint(), Find_NoContainer());
	if(!Injectable) return(0);
	
	SetColorDw(Injectable->~EssenceInfo(1)); //get and set color
	Essence = GetID(Injectable);
	Sound("StabExtract");
	SetAction("Filled");
}

public func DoInjection(){
	if(!Contained() || !Essence) return(0);
	AddEffect("Injected",Contained(),100,1,,SRIG,Essence);
	Sound("StabInject");
	RemoveObject();
}

//other
func Hit(){
	if(!Random(3)) Sound("Crystal*",0,this(),25);
}

//oooooooh! a syring effect!
public func FxInjectedStart(pTarget,iEffectNumber, iTemp, EssenceType){
	EffectVar(0,pTarget,iEffectNumber) = EssenceType;
}

public func FxInjectedTimer(pTarget, iEffectNumber, iEffectTime){
	if(iEffectTime > 23*36) return(-1);
	if(!Random(15) && iEffectTime > 3*36) CreateParticle("PSpark",GetX(pTarget)+RandomX(-8,8),GetY(pTarget)+8,0,-5,RandomX(25,40),EffectVar(0,pTarget,iEffectNumber)->~EssenceInfo(1),pTarget);
	if(iEffectTime != 3*36) return(0);
	var EssenceType = EffectVar(0,pTarget,iEffectNumber);
	DefinitionCall(EssenceType,"EssenceInfo",2,pTarget);
}

public func FxInjectedStop(pTarget,iEffectNumber, iTemp){
	var EssenceType = EffectVar(0,pTarget,iEffectNumber);
	DefinitionCall(EssenceType,"EssenceInfo",3,pTarget);
}

//oooh! info!
public func FxSyrinfInfo(object pTarget, int iEffectNumber){
	var i = Format("$TxtEssence$",Essence,GetName(,Essence));
	if(Essence) return(i);
}