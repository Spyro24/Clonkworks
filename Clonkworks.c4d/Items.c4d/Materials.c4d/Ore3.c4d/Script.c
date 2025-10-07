/*--- Eisenerz ---*/

#strict

func Hit () {
  Sound("RockHit*");
}

/* Für Erzmine */



func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(200); }
func SmeltResult(){ return(TTIG);} 
func IsMetal(){ return(true); }