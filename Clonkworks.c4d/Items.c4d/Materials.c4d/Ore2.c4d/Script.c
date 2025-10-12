/*--- Eisenerz ---*/

#strict

func Hit () {
  Sound("RockHit*");
}

/* Für Erzmine */

public func Sale(int plr) {
  var obj;
  if (obj = FindObject (COPM, 50 - GetX (), 49 - GetY ()))
    obj -> COPM::PlayerHasSold(plr, this ());
  return(GetID());
}

public func Purchase(int plr, object buyobj) {
  var obj;
  if (obj = FindObject (COPM, 50 - GetX (), 49 - GetY ()))
    obj -> COPM::PlayerHasBought(plr, this ());
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(200); }
func SmeltResult(){ return(CPIG);} 