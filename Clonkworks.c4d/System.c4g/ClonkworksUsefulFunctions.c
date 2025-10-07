/*-- Neues Script --*/

#strict 3

//Returns a random color
//Include Alpha - Alpha is also random
global func RandomRGBa(bool IncludeAlpha){
	if(IncludeAlpha) return(RGBa( RandomX(0,255) , RandomX(0,255) , RandomX(0,255) , RandomX(0,255)));
	return(RGBa( RandomX(0,255) , RandomX(0,255) , RandomX(0,255)));
}

//this function allows the creation of forged objects without the forge itself, useful for minigames or spawning a preforged item
//if an material is not on the forge list or the Forging Allowed rule is not in the scenario, this returns 0
//also, if the object you wanna spawn isnt forgeable, this will also return 0.
global func CreateForgedObject(id Id, int XOffset, int YOffset, int Owner, id MaterialId){
	var ListParent = FindObject2(Find_ID(FALW));
	var ok = false;
	var Index;
	
	if(!ListParent) return(0);
	
	for(var checkId = 0; checkId < GetLength(LocalN("ForgeableIDs", ListParent)); checkId++){
		if(LocalN("ForgeableIDs", ListParent)[checkId] == MaterialId){
			ok = true;
			Index = checkId;
			continue;
		}
	}
	
	if(!ok) return(0);
	if(!GetComponent(DUMM,,,Id)) return(0);
	
	var ForgedObj = CreateObject(Id, XOffset, YOffset, Owner);
	LocalN("Color", ForgedObj) = LocalN("ForgeableColors", ListParent)[Index];
	LocalN("Mass", ForgedObj) = LocalN("ForgeableMass", ListParent)[Index];
	LocalN("Speed", ForgedObj) = LocalN("ForgeableSpeed", ListParent)[Index];
	LocalN("Power", ForgedObj) = LocalN("ForgeablePower", ListParent)[Index];
	LocalN("Effect", ForgedObj) = LocalN("ForgeableEffect", ListParent)[Index];
	LocalN("MatId", ForgedObj) = MaterialId;
	
	ForgedObj->FRGUpdate();
	ForgedObj->AssignEffects();
	
	return(ForgedObj);
}

//same as above, but it creates a custom forge object with values passed into the function, instead of using a forge material. Not inspectable.
global func CreateForgedObjectCustom(id Id, int XOffset, int YOffset, int Owner, int Color, int Mass, int Speed, int Power, string Effect){
	if(!GetComponent(DUMM,,,Id)) return(0);
	
	var ForgedObj = CreateObject(Id, XOffset, YOffset, Owner);
	LocalN("Color", ForgedObj) = Color;
	LocalN("Mass", ForgedObj) = Mass;
	LocalN("Speed", ForgedObj) = Speed;
	LocalN("Power", ForgedObj) = Power;
	LocalN("Effect", ForgedObj) = Effect;
	LocalN("MatId", ForgedObj) = 0;
	
	ForgedObj->FRGUpdate();
	ForgedObj->AssignEffects();
	
	return(ForgedObj);
}