/*-- Neues Script --*/

#strict
#include CANN

protected func ControlConf(int conf)
{
  if(AimStdConf(conf))
    Sound("CatapultSet");
}

public func ControlUp(object clonk)     // Zielen: hoch (klassisch)
{
  [$TxtAimup$|Image=CAN1:2]
  AimUp(clonk, 2, "ControlConf");
}

public func ControlDig(object clonk)    // Zielen: runter (klassisch)
{
  [$TxtAimdown$|Method=Classic|Image=CAN1:0]
  AimDown(clonk, 2, "ControlConf");
}

public func ControlDown()
{
  [$TxtAimdown$|Method=JumpAndRun|Image=CAN1:0] // Beschreibung fuer JnR
}

public func ControlUpdate(object clonk, int comdir) // Zielen: JnR
{
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
  if(idObj != CK5P) return(0);
  // Noch jede Menge Platz
  if (ContentsCount() < 1) return(1);
  // Niemals mehr als 10 Objekte laden: Wird gebraucht, wenn aus ControlThrow aufgerufen!
  if (ContentsCount() >= 1) return(0);
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

public func RejectEntrance(pIntoObj){
	return(!AllowLoad(GetID(pIntoObj)));
}


private func MaxContents() { return(3); }