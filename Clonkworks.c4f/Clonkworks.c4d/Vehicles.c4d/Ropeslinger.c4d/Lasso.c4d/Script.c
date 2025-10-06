/*-- Neues Script --*/

#strict 2
local Mode;
local First;

func Initialize(){
	if(First) return(0);
	SetCon(25);
	Mode = false;
	First = 1;
}

func Check(){
	if(!Mode){
		DoCon(RandomX(1,2));
		if(GetCon() >= 100) Mode = true;
	}else{
		DoCon(-RandomX(0,1));
	}
	
	if(GBackSemiSolid(0,0)){
		CreateObject(CK5P,0,-3);
		RemoveObject(this());
	}
	
	var iTie = FindObject2(Find_AtPoint(0,0), Find_Or(Find_Category(C4D_Vehicle), Find_Category(C4D_Living)), Find_NoContainer());
	if(iTie){
		if(GetID(iTie) == RPSG) return(0);
		if(Mode == false && GetCon() < 30) return(0);
		var iRope = FindRope(this(), 2);
		var ConnectedTo = iRope->GetConnectedByRope(0);
		if(!iRope) return(0);
		if(CrewMember(iTie) >= 1 && Hostile(GetOwner(), GetOwner(iTie)) && FindObject(CR5P)){
			RemoveObject(iRope);
			iRope = CreateObject(CK5P);
			iRope->TieUp(,iTie);
			RemoveObject(iRope->GetConnectedByRope(1));
			iRope->ConnectObjects(ConnectedTo, iTie);
			RemoveObject();
			return(1);
		}
		iRope->ConnectObjects(iRope->GetConnectedByRope(0), iTie);
		RemoveObject();
		return(1);
	}
	
}

func Destruction(){
Sound("Click");
}

public func IsNotWeapon(){
	return(true);
}

