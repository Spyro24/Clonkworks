/*-- Neues Script --*/

#strict 3
#include HNCB

local DesignatedBumb2;

func Initialize() {
  return(1);
}

func Update(){
	if(DesignatedBumb)
	DesignatedBumb.Comb = 0;
	if(DesignatedBumb2)
	DesignatedBumb2.Comb = 0;
}
