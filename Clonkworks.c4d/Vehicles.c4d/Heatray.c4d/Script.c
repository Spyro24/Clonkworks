/*-- Neues Script --*/

#strict 2
#include CANN

protected func Initialize(){
	SetAction("Aim");
	SetPhase(8);
}

public func RayRange(){
	return(15);
}

public func Fire(bool fAuto)
{
  var pCoal=FindContents(COAL);
  if (!pCoal) 
  {
    Sound("Click");
    return(0);
  }
  
  if(IsHot()){
	  Message("$TooHot$",this());
	  Sound("Error");
	  return(1);
  }

  var iX = ((GetPhase()+GetPhase()/2)-8)*(GetDir()*2-1);
  if(iX < 0) iX = 0;
  var iY = (GetPhase()*2)-40;
  var iAngle = BoundBy(GetPhase()*5-6,0,90);
  var iXDir = Sin(iAngle,32*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-20)+GetPhase()-6;
  
  SetActionKeepPhase("OverheatHig");
  
  return(1);
}

protected func DoHeatCheck(){
	if(GetActTime() > 150){
		if(GetAction() == "OverheatHig"){
			SetActionKeepPhase("OverheatMed");
			return(0);
		}
		if(GetAction() == "OverheatMed"){
			SetActionKeepPhase("OverheatLow");
			return(0);
		}
		if(GetAction() == "OverheatLow"){
			SetActionKeepPhase("Aim");
		}
	}
	
	var Chance;
		if(GetAction() == "OverheatHig"){
			Chance = 2;
		}
		if(GetAction() == "OverheatMed"){
			Chance = 5;
		}
		if(GetAction() == "OverheatLow"){
			Chance = 10;
		}
		
	if(!Random(Chance) && GetAction() != "Aim") Smoke(-7*(GetDir()*2-1),0,RandomX(5,20));
}

protected func RejectCollect(id idObj, object pObj)
{
  return(!AllowLoad(idObj));
}

private func AllowLoad(id idObj)
{
	if(idObj != COAL) return(0);
  if (ContentsCount() < 12) return(1);
  return(0);
}

public func ALKConnectType(){
	return([FNPP]);
}

public func IsHot(){
	return(GetAction() != "Aim");
}

//aiming
public func ControlUp(object clonk)     // Zielen: hoch (klassisch)
{
  [$TxtAimup$|Image=CAN1:2]
  if(IsHot()){
	  Message("$TooHot$",this());
	  Sound("Error");
	  return(1);
  }
  AimUp(clonk, 4, "ControlConf");
}

public func ControlDig(object clonk)    // Zielen: runter (klassisch)
{
  [$TxtAimdown$|Method=Classic|Image=CAN1:0]
   if(IsHot()){
	  Message("$TooHot$",this());
	  Sound("Error");
	  return(1);
  }
  AimDown(clonk, 4, "ControlConf");
}

public func ControlDown()
{
  [$TxtAimdown$|Method=JumpAndRun|Image=CAN1:0] // Beschreibung fuer JnR
}

public func ControlUpdate(object clonk, int comdir) // Zielen: JnR
{
  if(IsHot() && (comdir == COMD_Up || comdir == COMD_Down)){
	  return(1);
  }
  AimUpdate(clonk, comdir, 4, "ControlConf");
}

public func ControlDownSingle() // Sicht zurücksetzen
{
  [$TxtResetview$|Method=None]
}