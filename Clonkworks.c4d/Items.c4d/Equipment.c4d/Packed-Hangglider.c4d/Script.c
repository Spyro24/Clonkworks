/*-- Neues Script --*/

#strict 2

func Initialize() {
  SetCon(150);
  SetAction("Idle");
  return(1);
}

func Incineration(){
	SetAction("Burnt");
	SetClrModulation(RGBa(45,45,45));
}

//reconstruction
public func Activate(pByObject){
	[$TxtPlace$]
	if(GetAction() == "Burnt"){
		Message("$TxtBurnt$",pByObject);
		Sound("WoodHit*");
		Smoke(0,0,RandomX(15,20),RGBa(45,45,45));
		RemoveObject(this());
		return(1);
	}
	
	Sound("Click");
	CreateObject(HNGG,0,10,GetOwner());
	RemoveObject();
	return(1);
}

func Hit(){
	if(!Random(3)) Sound("WoodHit*");
}