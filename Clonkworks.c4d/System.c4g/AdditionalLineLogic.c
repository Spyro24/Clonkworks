#strict

#appendto SPIP
#appendto DPIP
#appendto PWRL
#appendto FNPP
#appendto HOSE
#appendto TRPW

private func Transfer(){
	if(!ObjectCount(RSWR)) 	return(inherited());
	if(this()->~CustomLengthLimit()) return(inherited());
	var from, to;
	from = GetActionTarget(0);
	to = GetActionTarget(1);
	
	var Distance;
	Distance = 500;
	//pipes have a bigger range, so they can be used without pain.
	if(GetID() != PWRL) Distance = 1200;
	
	if(ObjectDistance(from,to) > Distance){
		LineBreak(false);
		RemoveObject(this());
	}
	
	return(inherited());
}

public func isLine(){ return(1); }
public func KitType(){
	if(GetID() == SPIP || GetID() == DPIP || GetID() == PWRL){
		return(LNKT);
	}else{
		return(_inherited());
	}
}
