#strict 2
#appendto *

public func IsNotWeapon(){
	return(_inherited());
}

func QueryCatchBlow(pByObject){
	if(pByObject->IsNotWeapon()){
		return(1);
	}
	
	return(_inherited(Par()));
}
