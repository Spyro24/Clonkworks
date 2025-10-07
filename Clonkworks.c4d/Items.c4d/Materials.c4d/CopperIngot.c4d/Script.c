/*-- Metall --*/

#strict

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}

// Wir leiten von LOAM ab, sind aber kein Alchemie-Container
public func IsAlchemContainer() { return(false); }

// Und auch kein Indianerzeug
public func IsIndianResearch() { return(false); }
public func IsIndianHandcraft() { return(false); }
func IsMetal(){ return(true); }