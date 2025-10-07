/*-- Vogelgezwitscher --*/

#strict

func Initialize(){
	for(var grs in FindObjects(Find_ID(GRAS))){
		SetGrass(grs);
	};
}

public func SetGrass(object grs){
		SetColorDw(RandomRGBa(true), grs);
		if (!Random(3)) SetPhase(2, grs);
		if (!Random(3)) SetPhase(3, grs);
}

