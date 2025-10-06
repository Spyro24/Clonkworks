/*-- Neues Szenario --*/

#strict 2

func Initialize() {
  FindObject2(Find_ID(FALW))->AddForgeable(METL, RGBa(177, 171, 170, 0), 3, 34, 2, "LavaLQM");
  FindObject2(Find_ID(FALW))->AddForgeable(CPIG, RGBa(170, 79, 66, 0), 4, 30, 3, "LavaLQM Lightning");
  FindObject2(Find_ID(FALW))->AddForgeable(TTIG, RGBa(120, 130, 139, 0), 2, 37, 0, "");
  FindObject2(Find_ID(FALW))->AddForgeable(ROCK, RGBa(190, 194, 192, 0), 6, 20, -1, "Fragile");
  FindObject2(Find_ID(FALW))->AddForgeable(GOLD, RGBa(255, 178, 18, 0), 6, 20, -1, "Fragile Glow");
  FindObject2(Find_ID(FALW))->AddForgeable(CRYS, RGBa(141, 169, 242, 127), 0, 45, 7, "Regen Glow Fragile");
  return(1);
}

func InitializePlayer(int iPlr){
	var ix, iy;
	ix = GetX(GetCrew(iPlr, 0))-100;
	iy = GetY(GetCrew(iPlr, 0));
	
	//getting the pole away from buildings
	if( FindObject2(Find_AtPoint(ix,iy), Find_Category(C4D_Structure)) ){
		while(FindObject2(Find_AtPoint(ix,iy), Find_Category(C4D_Structure))){
			ix -= 5;
		}
	}
	
	if(GBackSolid(ix, iy)){
		//Finding outside place for pole
		while(GBackSolid(ix, iy)){
			iy--;
		}
	}else{
		//Finding ground place for pole
		while(!GBackSolid(ix, iy)){
			iy++;
		}
	}
	
	var pole = CreateObject(CH5P, ix, iy);
	pole->ControlDigDouble();
	
	var bumb = CreateObject(BUMB, ix, iy-100);
	var rope = CreateObject(CK5P, ix, iy-25);
	rope->ConnectObjects(pole,bumb);
	
	var Names = ["Bumbington","Bobby","Gerald","Buzzer","Fatty","Hummington","Hummsman","Humm-Humm","Harold","Wocket Jr.","Krankhaft fettleibig"];
	SetName(Names[RandomX(0,GetLength(Names))], bumb);
	SetClrModulation(GetTeamColor(GetPlayerTeam(iPlr)), bumb);
}
