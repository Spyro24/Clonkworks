
#strict 2

func Initialize(){
	SetPosition(0,0);
}

func InitializePlayer(int iPlr){
	//welcome menu
	CreateMenu(BUMB,GetCrew(iPlr),this(),,"Clonkworks Playground 3",,3);
	AddMenuItem("$TxtWelcome$",,,GetCrew(iPlr));
	SetWealth(iPlr,999999);
}

public func Managment(){
	for(var i in FindObjects(Find_Category(C4D_Structure))){
		if(GetID(i) == KDNT) continue;
		if(GetCon(i) < 100 && !OnFire(i)) SetCon(100,i);
	}
}

public func Activate(pByPlr){
	var pByObj = GetCursor(pByPlr,0);
	CreateMenu(GetID(),pByObj,this(),0,GetName(),,1);
	AddMenuItem("$Opt1$","SpawnMenu",CRYS,pByObj,0,pByObj);
	AddMenuItem("$Opt2$","SpawnMenu",ROCK,pByObj,0,pByObj);
	AddMenuItem("$Opt3$","SpawnMenu",BUMB,pByObj,0,pByObj);
	AddMenuItem("$Opt4$","SpawnMenu",MTBK,pByObj,0,pByObj);
	AddMenuItem("$Opt5$","SpawnMenu",CLNK,pByObj,0,pByObj);
}

public func SpawnMenu(identifier, Spawner){
	var category = 0;
	if(identifier == ROCK) category = C4D_Object;
	if(identifier == BUMB) category = C4D_Living;
	if(identifier == MTBK) category = C4D_Vehicle;
	if(identifier == CLNK){
		CreateMenu(identifier,Spawner,this(),0,"Error :(");
		category = C4D_Living;
		for(var i = 0; GetDefinition(i,category) != 0; i++){
			var conk = GetDefinition(i,category);
			if(GetDefCoreVal("CrewMember","DefCore",conk) == 1){
			AddMenuItem(GetName(,GetDefinition(i,category)),"MorphTo",GetDefinition(i,category),Spawner,0,Spawner);
			}
		}
		return(0);
	};
	
	CreateMenu(identifier,Spawner,this(),0,"Error :(",,0,true);
	SetMenuSize(15,35,Spawner);
	for(var i = 0; GetDefinition(i,category) != 0; i++){
		AddMenuItem(GetName(,GetDefinition(i,category)),"Spawn",GetDefinition(i,category),Spawner,0,Spawner);
	}
}

public func Spawn(Item, Spawner){
	CreateObject(Item,GetX(Spawner),GetY(Spawner));
	Spawner->Sound("Ding");
}