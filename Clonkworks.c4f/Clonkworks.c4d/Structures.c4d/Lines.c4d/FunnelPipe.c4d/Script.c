/*--- Zuflussrohr ---*/

#strict

protected func Initialize()
{
  Local(0) = 1;
  Local(1) = 7;
	
  SetAction("Connect");  
  SetVertex(0, 0, GetX()); SetVertex(0, 1, GetY());
  SetVertex(1, 0, GetX()); SetVertex(1, 1, GetY());
  SetPosition(0, 0, this());
}

protected func Transfer()
{
  if(Contained()) Exit();
  var from = GetActionTarget(0);
  var to = GetActionTarget(1);
  
  if(from && to){
	  //getting max space
	  var space;
	  space = GetDefCoreVal("CollectionLimit", "DefCore", GetID(to));
	  if(space == 0) space = to->~MaxContents();
	  if(space == 0) space = 9999;
	  
	  if(GetAction(to) eq "DoorOpen") return(0); //im doing this so it doesnt get stuck on the open animation.
	  if(GetAction(from) eq "DoorOpen") return(0); // this too, apparently i forgot to do this previosuly
	  var MoveItem = FindObject2(Find_Container(from), Find_OCF(OCF_Collectible), Sort_Random());
	  if(GetOCF(to) & OCF_Container){
		  if(ContentsCount(,to) < space){
			  if(MoveItem != this() && to->~RejectEntrance(MoveItem) != 1)
			  Enter(to, MoveItem);
		  }
	  }
	  else if(GetOCF(to) & OCF_Collectible){
		  var cont;
		  cont = Contained(to);
		  if(GetOCF(cont) & OCF_Alive) return(0);
		  if(cont){
			 if(ContentsCount(,cont) < space){
			  if(MoveItem != this() && cont->~RejectEntrance(MoveItem) != 1)
			  Collect(cont, MoveItem);
			 }
		  }
		  else{
		  if(GetID(to) == FNKT) return(0);
		  Exit(MoveItem);
		  SetPosition(GetX(to), GetY(to), MoveItem);
		  }
	  }
  }
  
  if(Contained()) Exit();
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

public func KitType(){ return(FNKT); }
