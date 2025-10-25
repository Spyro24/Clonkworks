/*--- Zuflussrohr ---*/

#strict
local Dontdetect;

protected func Initialize()
{
  Local(0) = 70;
  Local(1) = 70;
	
  Dontdetect = [];
  SetAction("Connect");  
  SetVertex(0, 0, GetX()); SetVertex(0, 1, GetY());
  SetVertex(1, 0, GetX()); SetVertex(1, 1, GetY());
  SetPosition(0, 0, this());
}

public func DebugGet(){
	DebugLog("Not allowed: %v",Dontdetect);
	
	  var from = GetActionTarget(0);
		var to = GetActionTarget(1);
	var InLineObjects = FindObjects(Find_OnLine(GetX(from),GetY(from),GetX(to),GetY(to)), Find_Or(Find_Category(C4D_Vehicle), Find_Category(C4D_Object),Find_Category(C4D_Living)), Find_NoContainer(), Find_Exclude(from),Find_Exclude(to));
	DebugLog("Line: %v",InLineObjects);
}

protected func Transfer()
{
  if(Contained()) Exit();
  var from = GetActionTarget(0);
  var to = GetActionTarget(1);


  //Breaking if not straight. as this breaks the finding. 
  if(!PathFree(GetX(from),GetY(from),GetX(to),GetY(to))){
	  LineBreak(false);
	  RemoveObject(this());
	  return(0);
  }
  
  //custom break length for short lines
  if(ObjectCount(RSWR))
  if(ObjectDistance(from,to) > 300){
		LineBreak(false);
		RemoveObject(this());
		return(0);
  };
  
  var InLineObjects = FindObjects(Find_OnLine(GetX(from),GetY(from),GetX(to),GetY(to)), Find_Or(Find_Category(C4D_Vehicle), Find_Category(C4D_Object),Find_Category(C4D_Living)), Find_NoContainer(), Find_Exclude(from),Find_Exclude(to));
  var doActivate = false;
  //check for new objects
  for(var new in InLineObjects){
	  if(InArray(new,Dontdetect) == -1){
		  doActivate = true;
		  ArrayAdd(Dontdetect,new);
	  }
  }

  //check for objects not on line.
  for(var old in Dontdetect){
	  if(InArray(old,InLineObjects) == -1){
		  ArrayDeleteEntry(Dontdetect,old);
	  }
  }
  
  //activation
  if(!Contained(from) && !Contained(to) && doActivate)
  Activation();
  
}

public func Activation(){
	//DebugLog("Activated!");
	if(GetActTime() < 5) return(0);
	  var from = GetActionTarget(0);
	  var to = GetActionTarget(1);
	  
	  from->Sound("Click");
	  to->Sound("Click");
	  
	  from->~TripActivate();
	  to->~TripActivate();
}

public func LineBreak(bool fNoMsg)
{
  Sound("LineBreak");
  if (!fNoMsg) BreakMessage();
}

private func BreakMessage()
{
  var pPumpTarget = GetActionTarget(0);
  if (GetID(pPumpTarget) != FNKT)
    pPumpTarget = GetActionTarget(1);
  Message("$TxtLinebroke$", pPumpTarget);
}

public func CustomLengthLimit(){ return(1); }

public func KitType(){ return(FNKT); }
