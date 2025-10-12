/*--- Eisenerz ---*/

#strict
#appendto ORE1

func Hit () {
  Sound("RockHit*");
}

/* Für Erzmine */

public func Sale(int plr) {
  var obj;
  if (obj = FindObject (OREM, 50 - GetX (), 49 - GetY ()))
    obj -> OREM::PlayerHasSold(plr, this ());
  return(GetID());
}

public func Purchase(int plr, object buyobj) {
  var obj;
  if (obj = FindObject (OREM, 50 - GetX (), 49 - GetY ()))
    obj -> OREM::PlayerHasBought(plr, this ());
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(200); }
func SmeltResult(){ return(METL);} 