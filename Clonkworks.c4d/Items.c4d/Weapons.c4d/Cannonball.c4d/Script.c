/*-- Neues Script --*/

#strict 2
#include ROCK

local DNI; //If a thing gets hit, it gets added to this list so it doesnt get blown again.

func Initialize() {
	DNI = [];
  return(1);
}

func DoDestruct(){
	if(GetSpeed() < 100) return(0);
	
	for(var obj in FindObjects(Find_NoContainer(), Find_AtPoint(), Find_Or( Find_Category(C4D_Vehicle), Find_Category(C4D_Structure))) ){
		if(InArray(obj,DNI) != -1) continue;
		ArrayAdd(DNI,obj);
		DoCon(RandomX(-5,-1));
		
		var Dmg = RandomX(BoundBy(GetCon()-40,0,100),BoundBy(GetCon()-20,0,100));
		DoDamage(Dmg,obj);
		var Explo = BoundBy(GetCon()-70,5,100);
		
		if(obj){
		if(GetDefCoreVal("BlastIncinerate", "DefCore", GetID(obj))){
			if(GetDefCoreVal("BlastIncinerate", "DefCore", GetID(obj)) <= GetDamage(obj)){
				var Exploder = CreateObject(FLNT,AbsX(GetX(obj)),AbsY(GetY(obj)));
				Exploder->Explode(Explo);
			}else{
				obj->Sound("Blast2");
				DoExplosion(0,0,Explo);
			}
		}else{
			obj->Sound("Blast2");
			DoExplosion(0,0,Explo);
		}
		}
		
		for(var i = 0; i < RandomX(1,5); i++){
			Smoke(0,0,RandomX(20,40));
		}
		
		if(GetCon() <= 30) RemoveObject();
	}
}

func Hit(){
	DNI = [];
	return(_inherited());
}

public func IsChemicalProduct() { return(1); }
public func IsAdvancedProduct() { return(1); }