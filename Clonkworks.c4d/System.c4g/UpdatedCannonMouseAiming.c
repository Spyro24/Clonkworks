#strict 2

global func FxMouseAimTimer(object pTarget, int iEffectNumber){
	var clonk = FindObject2(Find_OCF(OCF_CrewMember), Find_Action("Push"), Find_ActionTarget(this()));
	if(!clonk) return(-1);
	if(LocalN("AutoAimTo",pTarget) > GetPhase()){
		SetPhase(GetPhase()+1);
		    Sound("CatapultSet");
	}
	else if(LocalN("AutoAimTo",pTarget) < GetPhase()){
		SetPhase(GetPhase()-1);
		    Sound("CatapultSet");
	}
	else{ 
		EffectVar(0,pTarget,iEffectNumber) = true;
	   return(-1);
	}
}

global func FxMouseAimStop(object pTarget, int iEffectNumber, int iReason){
	//DebugLog("%v",EffectVar(0,pTarget,GetEffect("MouseAim",pTarget,,0)));
	var clonk = FindObject2(Find_OCF(OCF_CrewMember), Find_Action("Push"), Find_ActionTarget(this()));
	if(!clonk) return(0);
	if(EffectVar(0,pTarget,iEffectNumber) != true) return(0);
	pTarget->~Fire(true);
}