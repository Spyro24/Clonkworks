/*-- Neues Objekt --*/

#strict 2

local Wild;

public func IsHoney(){ return(true); }
func Bendable(){ return(true); }

public func IsNotWeapon(){
	return(true);
}

func Initialize() {
	SetCon(RandomX(80,170));
  return(1);
}

func HoneyUpdate(){
	//dripping
	if(!Random(60) && !Contained())
	CreateParticle("HoneyDrip", 0, -0, RandomX(-2,2), 0, RandomX(10,25), RGBa(255,255,255));
}

func Entrance(object pContainer){
    SetAction("Idle");
	if(GetOCF(pContainer) & OCF_Alive){
		//find nearby bumbs and alert them if this honey isnt from the bumbhouse.
		if(Wild){
			for(var found in FindObjects(Find_ID(BUMB), Find_Distance(300))){
			if(found)
			if(ObjectDistance(this(), found) < 300 && GetAction(found) != "Rest")
			LocalN("GrudgeTarget", found) = pContainer;;
			}
		}
	}
}

public func Activate(object pClonk)
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

public func Eat(object pClonk)
{
  pClonk->~Feed(GetCon()/6);
  RemoveObject();
  return(1);
}

func ContactLeft() {
	if(GetAction() != "PickUp"){
		Sound("ArrowHit");
		SetAction("AttachLeft"); 
		return(1);
	}
}
	
func ContactRight() {
		if(GetAction() != "PickUp"){
		Sound("ArrowHit");
		SetAction("AttachRight"); 
		return(1);
	}
	}
	
func ContactBottom() {
		if(GetAction() != "PickUp"){
		Sound("ArrowHit");
		SetAction("AttachDown"); 
		return(1);
	}
	}
	
func ContactTop() {
		if(GetAction() != "PickUp"){
		Sound("ArrowHit");
		SetAction("AttachUp"); 
		return(1);
	} 
	}