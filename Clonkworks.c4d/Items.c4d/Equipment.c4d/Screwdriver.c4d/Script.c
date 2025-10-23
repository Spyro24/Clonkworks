/*-- Neues Script --*/

#strict 2

func Hit(){
	if(!Random(3)) Sound("MetalHit*");
}

public func Activate(pClonk){
	[$TxtUnscrew$]
	var Unscrewable = FindObjects(Find_Distance(30), Find_Func("Unscrewable"), Find_NoContainer(), Find_Exclude(this()));
	DebugLog("Found objects: %v",Unscrewable);
	if(Unscrewable == 0 || GetLength(Unscrewable) < 1){
		Message("$ErrNoObjects$",pClonk);
		pClonk->Sound("CommandFailure*");
		return(1);
	}
	DebugLog("OK!");
	
	if(GetLength(Unscrewable) == 1){
		Unscrew(,Unscrewable[0]);
		return(1);
	}
	
	CreateMenu(SCWD,pClonk,this(),0);
	for(var i in Unscrewable){
		AddMenuItem("$TxtUnscrew$: %s","Unscrew",GetID(i),pClonk,0,i);
	}
	
	return(1);
}

public func Unscrew(foo,pObj){
	if(!pObj) return(0);
	if(ObjectDistance(this(),pObj) > 30){
		var pClonk = ContainedTop(this());
		Message("$ErrObjectFar$",pClonk,GetName(pObj));
		pClonk->Sound("CommandFailure*");
		return(0);
	}
	
	pObj->~Unscrew();
	Sound("screw*");
	return(1);
}