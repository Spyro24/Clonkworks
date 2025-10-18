/*-- Statue --*/

#strict 3
#include IDOL

local StatueList;
local index;

protected func Hit3() {
  Sound("RockHit*");
  return(1);
}

func SayTele(){
	DrawParticleLine("MSpark", 0, 0, GetX(StatueList[index])-GetX(), GetY(StatueList[index])-GetY(), 20, 100, RGBa(127,0,255,50), RGBa(255,51,255,100), -8);
	Sound("Click");
	Message(GetName(StatueList[index]), this());
}

func FindTeles(){
	var newList = FindObjects(Find_ID(PGTP), Find_Exclude(this()));
	if(GetLength(StatueList) != GetLength(newList)){
		index = 0;
		StatueList = newList;
		SayTele();
	}
}

func Grabbed(object pByObject, bool fGrab){
	if(!fGrab) return(0);
	Sound("Ding");
	Message("$Instructions$", this());
}

func ControlDig(){
	[$Change$]
	if(index + 2 > GetLength(StatueList)) index = 0;
	else index++;
	
	SayTele();
}

func ControlThrow(){
	[$Tele$]
	Sound("Energize");
	StatueList[index]->Sound("Energize");
	var Obj = FindObjects(Find_NoContainer(),Find_Distance(50), Find_Exclude(this()), Find_Not(Find_ID(PGTP)), Find_Not( Find_Or(Find_Category(C4D_Structure), Find_Category(C4D_StaticBack)) ));
	for(var i in Obj){
		i->CastParticles("PxSpark", 10,100, 0,0, 25, 80, RGBa(255,255,0,0), RGBa(255,149,0,127));
		SetPosition(GetX(StatueList[index]), GetY(StatueList[index]), i);
		i->CastParticles("PxSpark", 10,100, 0,0, 25, 80, RGBa(255,255,0,0), RGBa(255,149,0,127));
	}
}