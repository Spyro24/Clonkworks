/*-- Neues Script --*/

#strict 2
#appendto CLNK

public func GetMaxSpecialCount(iIndex, fAmount)
{
  if(iIndex == 0) { if(fAmount) return(15); return("IsHoney"); }
  if(iIndex == 1) { if(fAmount) return(1); return("IsExtraItem"); }
}
