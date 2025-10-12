/*-- AdminCavern --*/

#strict 3

#include DOOR

//Admin Door Rules
local adLocked;
local adSafemode; 
local adFreeHealth;
local adInstantBuild;
local adSpawnAtDoor;
//environverment rulesets
local evBees;
//house
local house;


func Initialize() {
	house = FindObject(HUT3);
	adLocked = true; adSafemode = true;
	adFreeHealth = false; adInstantBuild = false; adSpawnAtDoor = false;
	evBees = false;
return(1);
}

public func ContextExtra(pCaller){
	[$OP1$|Image=SELV]
	CreateMenu(SELV, pCaller, this(),0,"$OP1$",,1);
	if(adLocked)
		AddMenuItem("$Lock$ ON", "TickVar", RACE, pCaller,0,"adLocked");
	else AddMenuItem("$Lock$ OFF", "TickVar", RACE, pCaller,0,"adLocked");
	
	if(GetOwner(pCaller) == 0){
	if(adSafemode)
		AddMenuItem("$Safe$ ON", "TickVar", KILC, pCaller,0,"adSafemode");
	else AddMenuItem("$Safe$ OFF", "TickVar", KILC, pCaller,0,"adSafemode");
	}
	
	if(adFreeHealth)
		AddMenuItem("$EndHealth$ ON", "TickVar", WPHT, pCaller,0,"adFreeHealth");
	else AddMenuItem("$EndHealth$ OFF", "TickVar", WPHT, pCaller,0,"adFreeHealth");
	if(adInstantBuild)
		AddMenuItem("$fastBuild$ ON", "TickVar", ENGI, pCaller,0,"adInstantBuild");
	else AddMenuItem("$fastBuild$ OFF", "TickVar", ENGI, pCaller,0,"adInstantBuild");
	if(adSpawnAtDoor)
		AddMenuItem("$doorspawn$ ON", "TickVar", STSN, pCaller,0,"adSpawnAtDoor");
	else AddMenuItem("$doorspawn$ OFF", "TickVar", STSN, pCaller,0,"adSpawnAtDoor");
}

public func ContextCreateRule(pCaller){
	[$OP2$|Image=GOAL]
	CreateMenu(GOAL, pCaller, this(),0,"$OP2$",,1);
	for(var rule in FindObjects(Find_Category(C4D_Rule))){
		if(GetID(rule) != TKNW && GetID(rule) != THBA && GetID(rule) != TACC && GetID(rule) != FALW)
			AddMenuItem(GetName(rule), "RemoveRule",GetID(rule), pCaller,0);
	}
	AddMenuItem("$Add$","AddRuleMenu",, pCaller,0,pCaller);
}

public func ContextCreateEnv(pCaller){
	[$OP3$|Image=EGLN]
	CreateMenu(EGLN, pCaller, this(),0,"$OP3$",,1);
	
	AddMenuItem("$inr$",,, pCaller,0,pCaller);
	if(!evBees)
	AddMenuItem("$AddBees$","EnableBees",BBCL, pCaller,0,pCaller);
    else AddMenuItem("$AddBees$ (ON)","Nope",BBCL, pCaller,0,pCaller);
	
	AddMenuItem("$inr2$",,, pCaller,0,pCaller);
	if(GetOwner(pCaller) == 0 || !adSafemode){
	AddMenuItem("$wave$","Unleash",MONS,pCaller,0);
	}
	if(ObjectCount(BUMB))
	AddMenuItem("$burn$","LightBumb",BUMB,pCaller,0);
	AddMenuItem("$wind$","WindUp",WMIL,pCaller,0);
	if(evBees) AddMenuItem("$skip$","TimeSkip",TIME,pCaller,0);
	AddMenuItem("$heat$","Heat",LAVA,pCaller,0,100);
	AddMenuItem("$cold$","Heat",ICE1,pCaller,0,-100);
	AddMenuItem("$lt$","Strike",FXL1,pCaller,0,-100);
}

public func Unleash(){
	Sound("Energize");
	for(var i = 0; i < RandomX(10,35); i++) SpawnCreature(MONS);
}

public func WindUp(){ SetWind(100); }
public func TimeSkip(){
	var time = FindObject(TIME);
	if(IsDay()) time->SetTime(30);
	else time->SetTime(0);
}
public func Heat(idType, int temp){ SetTemperature(temp); }

public func Strike(){
	LaunchLightning(Random(LandscapeWidth()), 0, -20, 41, +5, 15);
}

public func LightBumb(){
	Sound("Click");
	var bumbs = FindObjects(Find_ID(BUMB));
	Incinerate(bumbs[Random(GetLength(bumbs))]);
}

public func ContextCreateCreature(pCaller){
	[$OP4$|Image=WPHT]
	CreateMenu(EGLN, pCaller, this(),4,"$OP4$",,0);
	var Creaturelist = [WIPF, BUMB, BIRD, SNKE, SHRK, MONS, FISH, FMNS, DRGN];
	for(var creature in Creaturelist){
		if(GetName(,creature))
		AddMenuItem(GetName(,creature), "SpawnCreature",creature, pCaller,0);
	}
}

public func SpawnCreature(idItem){
	Sound("Ding");
	if(adSpawnAtDoor){
		var thingy = CreateObject(idItem);
		Enter(this(), thingy);
		SetCommand(thingy, "Exit");
	}else PlaceAnimal(idItem);
}

public func EnableBees(){
	evBees = true;
	CreateObject(TIME);
	CreateObject(EGRS);
	CreateObject(BBCL);
	CreateObject(BSES);
	CreateObject(REPR);
	for(var i = 0; i < RandomX(5,5000); i++){
		PlaceVegetation(GRAS, 0, 0, LandscapeWidth(), LandscapeHeight(), 100000);
	}
	Sound("Ding");
}

public func Nope(){
	Sound("Error");
	Message("$einr$");
}

public func TickVar(idItem, iParameter){
	if(LocalN(iParameter)) LocalN(iParameter) = false;
	else LocalN(iParameter) = true;
	Sound("Click");
}

public func RemoveRule(idItem){
	RemoveObject(FindObject(idItem));
	Sound("Click");
}

public func AddRuleMenu(idItem, iParameter){
	Sound("Click");
	CreateMenu(GOAL, iParameter, this(),4,"$Add$",,0);
	var AvailableRules = [COAN, CNMT, ENRG, FGRV, REAC, NMTT, RSWR, ALCO, MGES, NMGE, NTMG, NOIS, BCNR, RDKL];
	for(var rule in AvailableRules){
		if(!ObjectCount(rule) && GetName(,rule)) AddMenuItem(GetName(,rule), "AddRule",rule, iParameter,0);
	}
}

public func AddRule(idItem){
	Sound("Ding");
	CreateObject(idItem);
}

func SoundOpenDoor(){
	Sound("SteelGate*");
}

func SoundCloseDoor(){
	Sound("SteelGate*");
}

func Update(){
	for(var clonk in FindObjects(Find_OCF(OCF_CrewMember))){
			if(adFreeHealth){
					DoEnergy(10, clonk);
			}
		}
		
		if(adInstantBuild){
			for(var struc in FindObjects(Find_Category(C4D_Structure))){
				if(GetCon(struc) < 100) DoCon(1, struc);
			}
		}
		
	//endless content in house
	var items = [MGNT,GUNP,HTRY,RPSG,GOLD, ABRL, BARL, LBRL, OBRL, WBRL, CNKT, LNKT, FNKT, ASHS, COAL, CNCR, CRYS, ERTH,LIQG, LOAM, METL, CPIG, TTIG, ROCK, WOOD, ARWP, XARP, FARP, FBMP, FLNT, GUNP, SFLN, STFN, TFLN, EFLN, TRP1, BFLN, SBFN, SFN_, BALN, BLMP, CANN, CATA, XBOW, LORY, SLBT, STMG, SUB1, WAGN, HONY,CBAT, SPHR, ALC_, LQTK, BRDG, MTBK, ADM3, SBLW, PHNG, CPBT, CW5P, CK5P, SMJP, MDRL];
	for(var item in items){
		var e;
		if(!ContentsCount(item, house) && GetName(,item)){
			e = CreateObject(item);
			Enter(house, e);
			e->SetOwner(0);
		}
	}
}

func ActivateEntrance(pIntoObj){
	if(!adLocked) return(_inherited(pIntoObj));
	if(GetOwner(pIntoObj) == 0) return(_inherited(pIntoObj));
	
	PlayerMessage(GetOwner(pIntoObj), "$lockedentry$");
	return(1);
}