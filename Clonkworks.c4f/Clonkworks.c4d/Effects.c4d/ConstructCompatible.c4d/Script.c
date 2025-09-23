#strict 2

local iAttachedLeft, iAttachedRight; //Objects that have been attached

public func CanAttachBridge(){
	if(!ObjectCount(BCNR)) return(0);
	return(1); 
	} //can a bridge be attached to this?
public func LeftVert(){ return(0); } //Whats the left attachment verticy?
public func RightVert(){ return(0); } //Whats the right attachment verticy?

public func CanAttachLeft(){ //Can the left verticy be used for attachment?
	if(!iAttachedLeft) return(1);
}

public func CanAttachRight(){ //Can the right verticy be used for attachment?
	if(!iAttachedRight) return(1);
}

public func ClearLeft(){
	iAttachedLeft = 0;
}

public func ClearRight(){
	iAttachedRight = 0;
}