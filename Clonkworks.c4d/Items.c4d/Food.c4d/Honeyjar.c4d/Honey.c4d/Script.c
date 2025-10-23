/*-- Neues Objekt --*/

#strict 2

local Wild;


//public func IsBait(){ return(true); }

public func IsNotWeapon(){
	return(true);
}

func Initialize() {
	SetCon(RandomX(80,170));
  return(1);
}

func HoneyUpdate(){
	
	if(Wild && !GetEffect("WildHoney",this())){
		AddEffect("WildHoney",this(),20,0,this());
	}else if(GetEffect("WildHoney",this()) && !Wild){
		RemoveEffect("WildHoney");
	}
	
	//dripping
	if(!Random(60) && !Contained())
	CreateParticle("HoneyDrip", 0, -0, RandomX(-2,2), 0, RandomX(10,25), RGBa(255,255,255));
}

func FxWildHoneyInfo(object pTarget, int iEffectNumber){
	return("$TxtDanger$");
}

func Entrance(object pContainer){
    SetAction("Idle");
	if(GetOCF(pContainer) & OCF_Alive){
		//find nearby bumbs and alert them if this honey isnt from the bumbhouse.
		if(Wild){
			for(var found in FindObjects(Find_ID(BUMB), Find_Distance(300))){
			if(found)
			if(ObjectDistance(this(), found) < 300 && GetAction(found) != "Rest")
			LocalN("GrudgeTarget", found) = pContainer;;
			}
		}
	}
}

func ContactLeft() {
	if(GetAction() == "Idle"){
		//Sound("ArrowHit");
		SetAction("AttachLeft"); 
		return(1);
	}
}
	
func ContactRight() {
		if(GetAction() == "Idle"){
		//Sound("ArrowHit");
		SetAction("AttachRight"); 
		return(1);
	}
	}
	
func ContactBottom() {
		if(GetAction() == "Idle"){
		//Sound("ArrowHit");
		SetAction("AttachDown"); 
		return(1);
	}
	}
	
func ContactTop() {
		if(GetAction() == "Idle"){
		//Sound("ArrowHit");
		SetAction("AttachUp"); 
		return(1);
	} 
	}
	
//logic retained from arrow, allows packing
	
public func RejectEntrance(pInto)
{
 // Aus einem Fahrzeug direkt in die Basis? Dann nicht packen lassen
 // Hack, damit die Pfeile aus der Armbrust beim verkaufen richtig angerechnet werden
 if(Contained()) if (GetCategory(Contained()) & C4D_Vehicle)
   if (FindObject2(Find_ID(FLAG), Find_ActionTarget(pInto)))
     AddEffect("IntNoPacking", this, 1, 1);

 // Kann direkt nach Treffer nicht weggesammelt werden
 if(GetEffect("CannotBeCollected",this()))return(true);
 return(false);
}

public func Entrance(pClonk)
{
  // Wieder zu einem Objekt machen?
  if(!(GetCategory() & C4D_Object))
   SetCategory(C4D_Object,this());
   
  // Nicht im Pfeilpacket selber
  if(GetID(pClonk) == PackTo()) return(0);

  // Nicht wenn unterdrückt
  if(GetEffect("IntNoPacking", pClonk)) return(0);
  if(GetEffect("IntNoPacking", this)) return(0);
  if(GetEffect("IntArrowVanish", this())) return(0);

  // Hat der Clonk einen Köcher bei sich?
  var pObj;
  if(pObj = FindContents(QUIV,pClonk)) if(pObj->~CanPack(pClonk)) if(pObj->~AddPack(this())) return(1);

  // Zu einem Pack dazufügen
  return(Pack(pClonk));
}

public func JoinPack(pClonk,fForce)
{
  // Nicht wenn unterdrückt
  if(GetEffect("IntNoPacking", pClonk)) return(0);
  if(GetEffect("IntArrowVanish", this())) return(0);
  
  var pObj, aList;
  // Pfeilpakete im Clonk suchen
  aList = FindObjects(Find_Container(pClonk),Find_ID(PackTo()),Find_OCF(OCF_Fullcon));
  for(pObj in aList)
    // Noch Platz?
    if(pObj->~MaxPackCount() > pObj->~PackCount()) {
      // Integrieren
      pObj->~DoPackCount(1);
      return(RemoveObject());
    }
  // Nur bei passendem Flag Aufnahme erzwingen
  if(!fForce) return(0);
  // Neues Pack anfangen
  CreateContents(PackTo(),pClonk)->~SetPackCount(1);
  return(RemoveObject());
}

protected func Activate(pClonk)
{
  [$TxtPack$]
  return(Pack(pClonk));
}

public func Pack(pClonk)
{
  // Damit der Köcher dies nicht als eingesammeltes Pfeilpaket anerkennt
  var pObj, pQuiver;
  if(pQuiver = FindContents(QUIV,pClonk))
    AddEffect("IntNoPacking",pQuiver,1);
  // Andere Einzelpfeile suchen und packen
  for(pObj in FindObjects(Find_Exclude(this()),Find_Container(pClonk),Find_ID(GetID())))
    pObj->~JoinPack(pClonk,1);
  // und selber zu Pack hinzufügen
  JoinPack(pClonk,1);
  if(pQuiver)
    RemoveEffect("IntNoPacking",pQuiver);
  return(1);

}

protected func Sale(int iPlr)
{
  var cnt, cntpack = PackTo()->MaxPackCount(), idArrow = GetID();
  // Heimatbasispfeile zu Paketen zusammenfassen
  if (cntpack)
    {
    cnt = GetHomebaseMaterial(iPlr, idArrow) / cntpack;
    if (cnt)
      {
      DoHomebaseMaterial(iPlr, idArrow, -cnt*cntpack);
      DoHomebaseMaterial(iPlr, PackTo(), cnt);
      }
    }
}

public func FxCannotBeCollectedStart()
{
 return(1);
}

public func FxCannotBeCollectedTimer()
{
 return(-1);
}

public func PackTo() { return(HONJ); }