/*--- Abflussrohr ---*/

#strict

local hoseRope;

protected func Initialize()
{
  hoseRope = 0;
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
  
  if(pPumpSource && pPumpTarget && !hoseRope){
	  Local(0) = -1;
	  Local(1) = -1;
	  hoseRope = CreateObject(CK5P);
	  hoseRope->ConnectObjects(pPumpSource, pPumpTarget);
	  hoseRope->SetRopeLength(600);
	  LocalN("RopeColor", hoseRope) = RGBa(57,110,52);
	  LocalN("MaxLength", hoseRope) = 600;
	  LocalN("fNoPickUp_1", hoseRope) = 1;
	  LocalN("fNoPickUp_0", hoseRope) = 1;
	  return(0);
  }else if(pPumpSource != hoseRope->GetConnectedByRope(0) || pPumpTarget != hoseRope->GetConnectedByRope(1)){
  hoseRope->ConnectObjects(pPumpSource, pPumpTarget);
  LocalN("MaxLength", hoseRope) = 600;
  hoseRope->SetRopeLength(600);
  }
  
   //the source needs to be a liquid tank.
  if(pPumpSource->~IsLiquidStorage()){
	  //is the other end a wipfkit? turn it into a hose and bail.
	  if(GetID(pPumpTarget) == FNKT){
		  ChangeDef(HOSH,pPumpTarget);
		  pPumpTarget->Initialize();
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
		  
		  var minVal = (600-hoseRope->CalcLength())/4;
		  var MaxVal = minVal + (minVal/2);
		  pPumpTarget->Spew(amount, pPumpSource->GetLiquidType(), minVal, MaxVal);
	  }
  }
  
  return(0);
}

public func LineBreak(bool fNoMsg)
{
  if(hoseRope) RemoveObject(hoseRope);
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