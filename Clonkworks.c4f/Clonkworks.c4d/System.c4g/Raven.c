/*-- Ravens Funktionen --*/

#strict

global func GetHorizont(x, y)
{
  // Ist an dem Punkt bereits Erde/Flüssigkeit?
  if(GBackSemiSolid(x - GetX(), y - GetY()) )
  {
    // Dann suchen wir nach oben weiter
    for(var i = y; --i; i > 0)
    {
      // Keine Flüssigkeit mehr?
      if(!GBackSemiSolid(x - GetX(), i - GetY()) )
        // k, Horizont gefunden
        return(i);
    }
   
    // Erde reicht bis oben, aber oben kann man ja noch laufen
    // TODO: Fehler (-1) zurückgeben, wenn nicht oben offen? Macht das Sinn? *kA*
    return(i);
  }
  else
  {
    // Der Startpunkt ist in der Luft, also nach unten weitersuchen
    for(var i = y; i < LandscapeHeight()-1; ++ i)
    {
      // Erde?
      if(GBackSemiSolid(x - GetX(), i - GetY()) )
        // Passt, einen Pixel darüber, wo noch Himmel ist, zurückgeben
        return(i - 1);
    }
   
    // Sonst halt ganz unten. Macht wohl auch nicht soo viel Sinn, wenn unten offen ist.
    return(i);
  }
}
