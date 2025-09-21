/*--- Abflussrohr ---*/

#strict

protected func Initialize()
{
  Local(0) = 8;
  Local(1) = 8;
  
  SetAction("Connect");  
  SetVertex(0,0,GetX()); SetVertex(0,1,GetY());
  SetVertex(1,0,GetX()); SetVertex(1,1,GetY());
  SetPosition(0, 0, this());
}

private func Transfer()
{
  var pPumpSource = GetActionTarget(0);
  var pPumpTarget = GetActionTarget(1);
  if(!pPumpSource) return(0);
  //the source needs to be a liquid tank.
  if(pPumpSource->~IsLiquidStorage()){
	  //is the other end a wipfkit? turn it into a hose and bail.
	  if(GetID(pPumpTarget) == FNKT){
		  var newHose = pPumpTarget->CreateObject(HOSH);
		  SetActionTargets(pPumpSource, newHose, this());
		  RemoveObject(pPumpTarget);
		  return(0);
	  }
	  
	  //the hose must be on.
	  if(pPumpTarget->~IsHoseOn()){
		  var amount = 0; //will try to get 20, if it fails get a smaller number
		  for(var i = 0; i < 20; i++){
			  if(pPumpSource->HasLiquid()){
				  pPumpSource->SetLiquidAmount(pPumpSource->GetLiquidAmount()-1);
				  amount++;
			  }else{
				  break;
			  }
		  }
		  
		  pPumpTarget->Spew(amount, pPumpSource->GetLiquidType());
	  }
  }
  
  return(0);
}

public func LineBreak(bool fNoMsg)
{
  if(GetID(GetActionTarget(1)) == HOSH) ChangeDef(FNKT, GetActionTarget(1));
  Sound("LineBreak");
  if (!fNoMsg) BreakMessage();
}
  
public func BreakMessage()
{
  // Angeschlossene Objekte ermitteln
  var pPumpSource = GetActionTarget(0);
  var pPumpTarget = GetActionTarget(1);
  // Meldung bei Leitungsbausatz am einen oder anderen Ende ausgeben
  if (GetID(pPumpSource) == LNKT) 
    Message("$TxtLinebroke$", pPumpSource);
  else
    Message("$TxtLinebroke$", pPumpTarget);
}

public func KitType(){ return(FNKT); }