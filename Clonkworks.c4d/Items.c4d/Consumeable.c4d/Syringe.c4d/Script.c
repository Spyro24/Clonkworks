/*-- Neues Script --*/

#strict 2
local Essence;

func Initialize() {
  Update();
  return(1);
}

func Update(){
	if(GetAction() != "Filled"){
		SetPicture(17,22,33,41);
	}else{
		SetPicture(11,85,40,57);
	}
}

public func Activate(){
	[$TxtInject$]
	if(!Essence) SetAction("Idle");
	var NotFilled = true;
	if(GetAction() == "Filled") NotFilled = false;
	
	if(NotFilled){
	var Injectable = FindObject2(Find_Func("HasEssence"), Find_AtPoint(), Find_NoContainer());
	if(!Injectable){
		Message("$ErrNoCorpse$",Contained());
		Contained()->Sound("CommandFailure1");
		return(1);
	}
	
	//TODO: replace with effect
	Inject();
	}
	
	return(1);
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
	Sound("Corrode");
	SetAction("Filled");
}

public func DoInjection(){
	
}

//other
func Hit(){
	if(!Random(3)) Sound("Crystal*",0,this(),25);
}