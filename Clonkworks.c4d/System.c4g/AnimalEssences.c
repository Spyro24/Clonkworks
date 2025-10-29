/*-- Neues Script --*/

#strict 2
#appendto WIPF
#appendto MONS
#appendto FMNS
#appendto FISH
#appendto SHRK

public func HasEssence(){
	return(1);
}

public func EssenceInfo(int type, object pObj){
	//1 - Color
	//2 - Stats
	//3 - On End
	//4 - Timer
	
	if(GetID() == WIPF){
		if(type == 1) return(RGBa(224,148,12));
		if(type == 2 && pObj){
			var JumpOld, RunOld, DigOld;
			JumpOld = GetPhysical("Jump",0,pObj);
			RunOld = GetPhysical("Walk",0,pObj);
			DigOld = GetPhysical("Dig",0,pObj);
			
			SetPhysical("Jump",JumpOld+10000,3,pObj);
			SetPhysical("Walk",RunOld+10000,3,pObj);
			SetPhysical("Dig",DigOld+10000,3,pObj);
		}
		if(type == 3 && pObj){
			ResetPhysical(pObj,"Jump");
			ResetPhysical(pObj,"Walk");
			ResetPhysical(pObj,"Dig");
		}
		return(0);
	}
	
	if(GetID() == MONS){
		if(type == 1) return(RGBa(0,142,0));
		if(type == 2 && pObj){
			var ScaleOld, HangleOld, PunchOld;
			ScaleOld = GetPhysical("Scale",0,pObj);
			HangleOld = GetPhysical("Hangle",0,pObj);
			PunchOld = GetPhysical("Fight",0,pObj);
			RunOld = GetPhysical("Walk",0,pObj);
		
			SetPhysical("Scale",ScaleOld+30000,3,pObj);
			SetPhysical("Hangle",HangleOld+30000,3,pObj);
			SetPhysical("Fight",PunchOld+35000,3,pObj);
			SetPhysical("Walk",RunOld-10000,3,pObj);
		}
		if(type == 3 && pObj){
			ResetPhysical(pObj,"Scale");
			ResetPhysical(pObj,"Hangle");
			ResetPhysical(pObj,"Fight");
			ResetPhysical(pObj,"Walk");
		}
		return(0);
	}
	
	if(GetID() == FMNS){
		if(type == 1) return(RGBa(142,0,0));
		if(type == 2 && pObj){
			var ScaleOld, HangleOld, PunchOld;
			ScaleOld = GetPhysical("Scale",0,pObj);
			HangleOld = GetPhysical("Hangle",0,pObj);
			PunchOld = GetPhysical("Fight",0,pObj);
			RunOld = GetPhysical("Walk",0,pObj);
		
			SetPhysical("Scale",ScaleOld+30000,3,pObj);
			SetPhysical("Hangle",HangleOld+30000,3,pObj);
			SetPhysical("Fight",PunchOld+35000,3,pObj);
			SetPhysical("Walk",RunOld-10000,3,pObj);
			
			Incinerate(pObj);
		}
		if(type == 3 && pObj){
			ResetPhysical(pObj,"Scale");
			ResetPhysical(pObj,"Hangle");
			ResetPhysical(pObj,"Fight");
			ResetPhysical(pObj,"Walk");
		}
		return(0);
	}
	
	if(GetID() == FISH || GetID() == SHRK){
		if(type == 1) return(RGBa(255,255,255));
		if(type == 2 && pObj){
			var SwimOld, PunchOld;
			SwimOld = GetPhysical("Swim",0,pObj);
			PunchOld = GetPhysical("Fight",0,pObj);
			
			SetClrModulation(RGBa(210,255,210),pObj);
			SetPhysical("Swim",SwimOld+35000,3,pObj);
			if(GetID() == FISH) SetPhysical("BreatheWater",1,3,pObj);
			if(GetID() == SHRK) SetPhysical("Fight",PunchOld+10000,3,pObj);
		}
		if(type == 3 && pObj){
			ResetPhysical(pObj,"Swim");
			ResetPhysical(pObj,"BreatheWater");
			if(GetID() == SHRK) ResetPhysical(pObj,"Fight");
			if(GetClrModulation(pObj) == RGBa(210,255,210)){
				SetClrModulation(0,pObj);
			}
		}
		return(0);
	}
}