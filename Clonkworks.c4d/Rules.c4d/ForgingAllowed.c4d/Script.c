//This rule is used by the forge to fetch materials it can use, while also making it easy to use without modifying object code.

#strict

local ForgeableIDs;
local ForgeableColors;
local ForgeableMass;
local ForgeableSpeed;
local ForgeablePower;
local ForgeableEffect;

protected func Initialize()
{
  ForgeableIDs = [];
  ForgeableColors = [];
  ForgeableMass = [];
  ForgeableSpeed = [];
  ForgeablePower = [];
  ForgeableEffect = [];
}

public func AddForgeable(id ObjectID, int Color, int Mass, int Speed, int Power, string EffectString){
	//adding a new spot for a new forgeable
	var newspot = GetLength(ForgeableIDs)+1;
	SetLength(ForgeableIDs, GetLength(ForgeableIDs)+1);
	SetLength(ForgeableColors, GetLength(ForgeableColors)+1);
	SetLength(ForgeableMass, GetLength(ForgeableMass)+1);
	SetLength(ForgeableSpeed, GetLength(ForgeableSpeed)+1);
	SetLength(ForgeablePower, GetLength(ForgeablePower)+1);
	SetLength(ForgeableEffect, GetLength(ForgeableEffect)+1);
	
	//adding the new values
	ForgeableIDs[newspot] = ObjectID;
	ForgeableColors[newspot] = Color;
	ForgeableMass[newspot] = Mass;
	ForgeableSpeed[newspot] = Speed;
	ForgeablePower[newspot] = Power;
	ForgeableEffect[newspot] = EffectString;
	
	DebugLog("Added (%i, ColorID: %d, Mass: %d, Speed: %d, Power: %d, Effects: %s) to Forgeables", ForgeableIDs[newspot], ForgeableColors[newspot], ForgeableMass[newspot], ForgeableSpeed[newspot], ForgeablePower[newspot], ForgeableEffect[newspot]);
}

public func RemoveForgeable(id ObjectID){
	for(var i = 0; i < GetLength(ForgeableIDs); i++){
		if(ForgeableIDs[i] == ObjectID){
			ForgeableIDs[i] = 0;
			return(1);
		}
	}
	return(0);
}


protected func Activate(caller) {
	
	if(ObjectCount(NOIS)){
	MessageWindow(GetDesc(), caller);
	return(1);
	}
	
  var obj = GetCursor(caller);
  CreateMenu(FALW, obj, 0, 0, "$Inspect$");
  for(var i in ForgeableIDs){
	  if(GetName(,i) && i)
		  AddMenuItem(GetName(,i), "Inspect", i, obj,0,obj);
  }
  return(1);
}

public func Inspect(iItemId, pCaller){
	//finding the item in the list
	
	if(!iItemId || !GetName(,iItemId)){
		PlayerMessage(GetOwner(pCaller), "$Err$");
		Sound("Error", true, pCaller, 0, GetOwner(pCaller));
		return(0);
	}
	
	var i = 0;
	var found = false;
	var mass, sped, powr, efft;

	for(var itm in ForgeableIDs){
		if(itm == iItemId){
			found = true;
				mass = ForgeableMass[i];
				sped = ForgeableSpeed[i];
				powr = ForgeablePower[i];
				efft = ForgeableEffect[i];
			continue;
		}
		i++;
	}
	
	//if didnt find the item, notify the caller and quit
	if(!found){
		PlayerMessage(GetOwner(pCaller), "$Err$");
		Sound("Error", true, pCaller, 0, GetOwner(pCaller));
		return(0);
	}
	
	CreateMenu(INSP, pCaller, 0, 0, "$Inspect$",,1);
	
	AddMenuItem("$Lab1$",,, pCaller);
	 AddMenuItem(GetName(,iItemId),, iItemId, pCaller);
	AddMenuItem("$Lab2$",,, pCaller);
	
	if(mass >= 5) AddMenuItem(Format("$Mass$", mass, "$Bad$"),,XMSS, pCaller);
	else if(mass < 5 && mass > 2) AddMenuItem(Format("$Mass$", mass, "$Ok$"),,XMSS, pCaller);
	else AddMenuItem(Format("$Mass$", mass, "$Good$"),,XMSS, pCaller);
	
	if(sped >= 36) AddMenuItem(Format("$Speed$", sped, "$Good$"),,XSPD, pCaller);
	else if(sped < 36 && sped > 30) AddMenuItem(Format("$Speed$", sped, "$Ok$"),,XSPD, pCaller);
	else AddMenuItem(Format("$Speed$", sped, "$Bad$"),,XSPD, pCaller);
	
	if(powr >= 5) AddMenuItem(Format("$Power$", powr, "$Good$"),,XPOW, pCaller);
	else if(powr < 5 && powr > 1) AddMenuItem(Format("$Power$", powr, "$Ok$"),,XPOW, pCaller);
	else AddMenuItem(Format("$Power$", powr, "$Bad$"),,XPOW, pCaller);
	
	//Statistcs
	AddMenuItem("$Lab3$",,, pCaller);
	
	if(WildcardMatch(efft, "*Burnable*")) AddMenuItem("$Burnable$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Fuming*")) AddMenuItem("$Fuming$",,, pCaller);
	if(WildcardMatch(efft, "*Windy*")) AddMenuItem("$Windy$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Float*")) AddMenuItem("$Float$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*LQM*")) AddMenuItem("$LQM$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*WaterLQM*")) AddMenuItem("$WaterLQM$",,, pCaller);
	if(WildcardMatch(efft, "*LavaLQM*")) AddMenuItem("$LavaLQM$",,, pCaller);
	if(WildcardMatch(efft, "*AcidLQM*")) AddMenuItem("$AcidLQM$",,, pCaller);
	if(WildcardMatch(efft, "*OilLQM*")) AddMenuItem("$OilLQM$",,, pCaller);
	if(WildcardMatch(efft, "*Regen*")) AddMenuItem("$Regen$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Glide*")) AddMenuItem("$Glide$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Toxic*")) AddMenuItem("$Toxic$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Skew*")) AddMenuItem("$Skew$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Glow*")) AddMenuItem("$Glow$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Explode*")) AddMenuItem("$Explode$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Fragile*")) AddMenuItem("$Fragile$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Lightning*")) AddMenuItem("$Lightning$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Sticky*")) AddMenuItem("$Sticky$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Bouncy*")) AddMenuItem("$Bouncy$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Attractive*")) AddMenuItem("$Attractive$",,XPNT, pCaller);
	if(WildcardMatch(efft, "*Magnet*")) AddMenuItem("$Magnet$",,XPNT, pCaller);
}

