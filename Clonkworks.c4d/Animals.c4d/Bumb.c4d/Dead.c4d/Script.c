/*-- Toter Wipf --*/

#strict

/* Verwesung */

private func Decaying()
{
  DoCon(-4);
}
  
/* Aktionen von außen */  
  
public func Eat(pBy)
{
  pBy->~Feed(10);
  RemoveObject();
  return(1);
}

