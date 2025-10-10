/*-- Neues Script --*/

#strict 2
#include CANN

protected func Initialize(){
	SetAction("Aim");
	SetPhase(8);
}

public func Departure(){
	SetPosition(GetX(),GetY()+20);
}

public func RayRange(){
	return(120);
}

public func RayBlastRange(){
	return(20);
}

protected func UseFuel(){
	for(var i = 0; i < 2; i++){
		RemoveObject(FindObject2(Find_Container(this()), Find_ID(COAL)));
	}
	return(1);
}

public func Fire(bool fAuto)
{
  var pCoal=ObjectCount2(Find_Container(this()), Find_ID(COAL));
  if (pCoal < 2) 
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
  var iYDir = Cos(iAngle,-19)+GetPhase()-16;
  
  iXDir/=4;
  iYDir/=4;
  
  for(var i = 2; i < RayRange(); i++){
	  var rX = iX+(iXDir*i);
	  var rY = iY+(iYDir*i);
	  //DebugLog("%d %d",rX,rY);
	  //is there an object in the way? burn that instead of landscape.
	  if(FindObject2(Find_OnLine(iX,iY,iX+(iXDir*i),iY+(iYDir*i)), Find_OCF(OCF_Inflammable), Find_Exclude(this()), Find_NoContainer())){
		  var e = FindObject2(Find_OnLine(iX,iY,iX+(iXDir*i),iY+(iYDir*i)), Find_OCF(OCF_Inflammable), Find_Exclude(this()), Find_NoContainer());
		  Incinerate(e);
		  Punch(e, RandomX(10,20));
		  DrawParticleLine("PSpark",iX,iY,GetX(e)-GetX(),GetY()-GetY(e),3,75,RGBa(255,64,0), RGBa(255,0,0),0);
		  CastParticles("PSpark", 10,100, iX+(iXDir*i), iY+(iYDir*i), 50, 85, RGBa(255,0,0), RGBa(255,64,0));
		  SetActionKeepPhase("OverheatHig");
		  Sound("laser*");
		  UseFuel();
		  return(1);
	  }
	  
	  //loosening material
	  if(GBackSolid(iX+(iXDir*i), iY+(iYDir*i)) || GBackLiquid(iX+(iXDir*i), iY+(iYDir*i))){
		  if(FlameConsumeMaterial(iX+(iXDir*i), iY+(iYDir*i))){
			  IncinerateLandscape(iX+(iXDir*i), iY+(iYDir*i));
			  DrawParticleLine("PSpark",iX,iY,iX+(iXDir*i), iY+(iYDir*i),3,75,RGBa(255,64,0), RGBa(255,0,0),0);
			  CastParticles("PSpark", 10,100, iX+(iXDir*i), iY+(iYDir*i), 50, 85, RGBa(255,0,0), RGBa(255,64,0));
			  SetActionKeepPhase("OverheatHig");
			  Sound("laser*");
			  UseFuel();
			  return(1);
		  }else{
			  Loosen(iX+(iXDir*i),iY+(iYDir*i),RayBlastRange());
			  DrawParticleLine("PSpark",iX,iY,iX+(iXDir*i), iY+(iYDir*i),3,75,RGBa(255,64,0), RGBa(255,0,0),0);
			  CastParticles("PSpark", 10,100, iX+(iXDir*i), iY+(iYDir*i), 50, 85, RGBa(255,0,0), RGBa(255,64,0));
			  SetActionKeepPhase("OverheatHig");
			  Sound("laser*");
			  UseFuel();
			  return(1);
		  }
	  }
  }
  
  Message("$NothinginSight$",this());
  Sound("Error");
  return(1);
}

protected func DoHeatCheck(){
	if(GetCon() < 100) return(0);
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


public func ControlCommand(string szCommand,object pTarget,int iX,int iY)
{
  // Feuern
  if(szCommand == "MoveTo")
    if(Distance(GetX(),GetY(),iX,iY)>50)
    {
		if(IsHot()){
				Message("$TooHot$",this());
				Sound("Error");
				return(1);
		}
      HaltPushers();
      return(FireAt(iX,iY));
    }
}

public func IsAdvancedProduct(){
	return(1);
}

public func ExitWorkshop(){
	if(GetCon() < 100) return(false);
	return(true);
}