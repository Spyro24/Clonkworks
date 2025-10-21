/*-- Neues Script --*/

#strict
#include CANN

protected func ControlConf(int conf)
{
	 if(GetEffect("MouseAim",this())) return(0);
  if(AimStdConf(conf))
    Sound("CatapultSet");
}

public func ControlUp(object clonk)     // Zielen: hoch (klassisch)
{
  [$TxtAimup$|Image=CAN1:2]
   if(GetEffect("MouseAim",this())) return(0);
  AimUp(clonk, 2, "ControlConf");
}

public func ControlDig(object clonk)    // Zielen: runter (klassisch)
{
  [$TxtAimdown$|Method=Classic|Image=CAN1:0]
   if(GetEffect("MouseAim",this())) return(0);
  AimDown(clonk, 2, "ControlConf");
}

public func ControlDown()
{
  [$TxtAimdown$|Method=JumpAndRun|Image=CAN1:0] // Beschreibung fuer JnR
}

public func ControlUpdate(object clonk, int comdir) // Zielen: JnR
{
	 if(GetEffect("MouseAim",this())) return(0);
  AimUpdate(clonk, comdir, 2, "ControlConf");
}


public func Fire(bool fAuto)
{
  var pProjectile=FindOtherContents(GUNP);
  // Schießpulver oder Projektil fehlt
  if (!pProjectile) 
  {
    Sound("Click");
    return(0);
  }

  // Austritt berechnen
  var iX = (GetPhase()+5)*(GetDir()*2-1);
  var iY = GetPhase()-18;
  var iAngle = BoundBy(GetPhase()*5-5,0,90);
  var iXDir = Sin(iAngle,32*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-18);
  
  iYDir /= 2;
  iXDir /= 2;
  
  if(GetID(pProjectile) == CW5P){
	  var rope;
	  while(rope = FindRope(pProjectile,0)){
		  RemoveObject(rope);
	  }
	  Exit(pProjectile,iX,iY,Random(360),iXDir,iYDir,+30);
	  pProjectile->Launch(pProjectile);
	  
	  var iRope = FindRope(pProjectile,0);
	  RemoveObject(iRope->GetConnectedByRope(0));
	  iRope->ConnectObjects(this(),pProjectile);
	  iRope->SetRopeLength(3000);
	  return(1);
  }
  
  RemoveObject(pProjectile);
  pProjectile = CreateObject(LSSO);
  Enter(this(), pProjectile);
  
  var Rope = CreateObject(CK5P);
  Rope->ConnectObjects(this(), pProjectile);
  Rope->SetRopeLength(3000);

  // Projektil abfeuern
  Exit(pProjectile,iX,iY,Random(360),iXDir,iYDir,+30);

  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pProjectile);
  // Sound
  Sound("Catapult");
  // Erfolgreich geschossen
  return(1);
}

private func AllowLoad(id idObj)
{
  if(idObj != CK5P && idObj != CW5P) return(0);
  // Noch jede Menge Platz
  if (ContentsCount() < 3) return(1);
  // Niemals mehr als 10 Objekte laden: Wird gebraucht, wenn aus ControlThrow aufgerufen!
  if (ContentsCount() >= 4) return(0);
  return(1);
}

private func AdjustLook(){
	if(ContentsCount())
	SetGraphics("Rope", this());
    else SetGraphics();
}

public func ALKConnectType(){
	return([FNPP]);
}

//for funnel pipe
public func CanNotBeDispensedInto(MoveItem){
	return(!AllowLoad(GetID(MoveItem)));
}


private func MaxContents() { return(3); }

public func Departure(){
	SetPosition(GetX(),GetY()+17);
}

public func GetResearchBase() { return(CANN); }

public func FireAt(int iX,int iY,int fAuto)
{
  var iAngle;
  // Zielwinkel
  iAngle = Angle(GetX(),GetY(),iX,iY);
  // Bei größerer Distanz höher zielen
  if(Inside(iX-GetX(),+1,+300))
     iAngle -= Abs(iX-GetX())/12;
  if(Inside(iX-GetX(),-300,-1))
     iAngle += Abs(iX-GetX())/12;
  // Zielen
  AimToAngle(iAngle);
  // Feuern
    if(!GetEffect("MouseAim",this()))
  AddEffect("MouseAim",this(),100,2,this());
  return(1);
}