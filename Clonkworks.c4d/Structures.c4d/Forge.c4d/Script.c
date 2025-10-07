/*-- Forge --*/
// the forge's way of crafting is different from a typical crafting building like the workshop.
// if youre making any mods in the future, dont take this building as inspiration, the code here is old and ass :(

#strict

#include DOOR
#include BAS4
#include CXEC

local Forgebase;
local Mat;

local Prod;

local Producing;
local Forging;
local len;

func CanProduce(){return(!Producing && Forgebase != 0 && !Forging);}
func IsProducing(){return(Producing && !Forging);}

local Worker;

local x;
local y;

local ForgeTimer;

local LinkedToAdv;

func Initialize() {
ForgeTimer = 0;
Producing = false;
Forging=false;
Forgebase = FindObject2(Find_ID(FALW));
}

func CanInspect(){ return(ObjectCount(FALW) && !ObjectCount(NOIS)); }

public func ContextInspect(pCaller){
	[$Inspect$|Image=INSP|Condition=CanInspect]
	FindObject(FALW)->Activate(GetOwner(pCaller));
}


func ContextForge(object pByObject){
	[$CtxForge$|Image=CXCN|Condition=CanProduce]
		CreateMenu(CXCN,pByObject,this(),4,"$MsgNoUseable$");
		for(var id in LocalN("ForgeableIDs", Forgebase)){
			if(!id) continue;
			AddMenuItem("$ForgeUseMat$: %s","MatSelected",id,pByObject,0,pByObject);
		}
}

func ContextStartContinue(pByObject){
	[$CtxCont$|Image=CXCN|Condition=IsProducing]
	if(pByObject == Worker){
		y = 0;
		x = GetComponent(,y,,Prod);
	}else{
		y -= 1;
	}
	Continue(pByObject);
}

func Continue(pByObject){
	
	//very big fat algorhythm for finding out if any components are still needed
	//very obese

	if(!Forging){
		
		//finding out the amount of unique components
		len = 0;
		var b = 1;
		for(var i = 0; b != 0; i++){
			b = GetComponent(,i,,Prod);
			if(b){
				len += 1;
			}
		}
		
		DebugLog("Total Length of Unique Components in %s: %d", GetName(,Prod), len);
		
	x = GetComponent(,y,,Prod);
	//y++;
	
	var Amt = GetComponent(x, , , Prod);
	if(x == DUMM){ x = Mat;
	Amt += GetComponent(x, , , Prod);
	}
	if(!Producing) return(1);
	if(x){
		DebugLog("Finding %s", GetName(,x));
		if(ContentsCount(x, this()) < Amt){
			AddCommand(pByObject, "Wait", , , , , 15);
				if(GetCategory(FindObject2(Find_ID(x))) & C4D_Vehicle()){
					Message("$Move$",pByObject,GetName(,x), Amt-ContentsCount(x, this()));
					AddCommand(pByObject, "Call",this(), , , , , "Continue", 3);
					for(var i = 0; i < Amt-ContentsCount(x, this()); i++){
					AddCommand(pByObject, "Enter", this());
					AddCommand(pByObject, "PushTo", FindObject2(Find_ID(x)), , , this(), , , 3);
					}
					y++;
					return(0);
				}else{
					Message("$Gather$",pByObject,GetName(,x), Amt-ContentsCount(x, this()));
					AddCommand(pByObject, "Call",this(), , , , , "Continue", 3);
					for(var i = 0; i < Amt-ContentsCount(x, this()); i++){
					AddCommand(pByObject, "Put", this());
					AddCommand(pByObject, "Enter", this());
					AddCommand(pByObject, "Acquire", , , , this(), , x);
					}
					y++;
					return(0);
				}
		}
		y++;
		Continue(pByObject);
		return(0);
		}
		
		if(Contained(pByObject) == this()){
			if(pByObject == Worker){
			var v;
			DebugLog("Started Looking for components, len: %d", len);
			for(var i = 0; i < len; i++){
				v = GetComponent(,i,,Prod);
				Amt = GetComponent(v, , , Prod);
				if(v == DUMM) v = Mat;
				DebugLog("%d %s needed, has %d", Amt, GetName(,v), ContentsCount(v, this()));
				if(ContentsCount(v, this()) < Amt){
					DebugLog("Forging Interrupted: Missing Items");
					Message("$GatherFail$",pByObject);
					Sound("CommandFailure", false, pByObject);
					return(0);
				}
			}
			StartWork(pByObject);
			}else{
				Message("$WrongWorker$",pByObject,GetName(Worker));
				Sound("CommandFailure", false, pByObject);
			}
		}
	    else{
		AppendCommand(pByObject, "Exit");
		AppendCommand(pByObject, "Call",this(), , , , , "ContextStartContinue", 3);
		return(0);
		}
	}

		
	}


func ContextScrap(pByObject){
	[$CtxScrap$|Image=WKS2|Condition=IsProducing]
	DebugLog("Forging Interrupted: Scrapped");
	Reset();
}

func FreeWork(){
	Sound("Discharge");
	OpenEntrance();
	for(var i = 0; i < ObjectCount2(Find_OCF(OCF_CrewMember), Find_Container(this())); i++){
		if (GetController(FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this()))[i]) == -1){
			SetController( GetOwner(FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this()))[i]), FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this()))[i]);
		}
	}
	
	SetObjectStatus(1, Worker, false);
	SetAction("Idle");
	CloseEntrance();
}

func Reset(){
	Producing = false;
	Forging = false;
	Mat = 0;
	Prod = 0;
	Worker = 0;
	x = 0;
	y = 0;
}

func FreeWorkF(){
	FreeWork();
}

protected func ActivateEntrance(pObj)
{
  if (ActIdle()) SetAction("OpenDoor");
  return(1);
}
	
func FinishWork(){
	FreeWork();
	var NewItem = CreateObject(Prod);
	Enter(this(), NewItem);
	
	var Color;
	var Mass;
	var Speed;
	var Power;
	var Efx;
	
	for(var i = 0; i < GetLength(LocalN("ForgeableColors", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Color = LocalN("ForgeableColors", FindObject2(Find_ID(FALW)))[i];
	}
	
	for(var i = 0; i < GetLength(LocalN("ForgeableMass", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Mass = LocalN("ForgeableMass", FindObject2(Find_ID(FALW)))[i];
	}
	
		for(var i = 0; i < GetLength(LocalN("ForgeableSpeed", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Speed = LocalN("ForgeableSpeed", FindObject2(Find_ID(FALW)))[i];
	}
	
		for(var i = 0; i < GetLength(LocalN("ForgeablePower", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Power = LocalN("ForgeablePower", FindObject2(Find_ID(FALW)))[i];
	}
	
	for(var i = 0; i < GetLength(LocalN("ForgeableEffect", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Efx = LocalN("ForgeableEffect", FindObject2(Find_ID(FALW)))[i];
	}
	
	
	LocalN("Color", NewItem) = Color;
	LocalN("Mass", NewItem) = Mass;
	LocalN("Speed", NewItem) = Speed;
	LocalN("Power", NewItem) = Power;
	LocalN("Effect", NewItem) = Efx;
	LocalN("MatId", NewItem) = Mat;
	
	SetOwner(GetOwner(), NewItem);
	NewItem->FRGUpdate();
	NewItem->AssignEffects();
	
	Message("$ForgeSuccess$", this(), GetName(,Prod));
	Reset();
}

func MatSelected(idType, pByObject){
	Mat = idType;
	Worker = pByObject;
	MenuProduction(pByObject);
	return(0);
}

private func MenuProduction(pCaller) {
  CreateMenu(CXCN,pCaller,this(),1,"$NoPlrKnowledge$");
  for(var i=0,idKnowledge; idKnowledge=GetPlrKnowledge(GetOwner(pCaller),0,i, C4D_All); ++i)
  {
	if(GetComponent(DUMM, , , idKnowledge) == 0)
		continue;
	if(idKnowledge->~IsAdvancedProduct() && !AdvancedResearchAllowed(GetOwner(pCaller))) continue;
    AddMenuItem("$Construction$: %s", "Gather", idKnowledge, pCaller, 0, pCaller);
  }
  return(1);
}

func Gather(idType, pCaller){
	for(var i in FindObjects(Find_Container(this()), Find_OCF(OCF_CrewMember))){
		CloseMenu(i);
	}
	Prod = idType;
	Producing = true;
	ContextStartContinue(pCaller);
}

func StartWork(User){	
	CloseEntrance();
	//clearing all needed components from the building's contents
	var z;
	z = 1;
	y = 0;
	var Amt;
	var Spec;
	
	var EmergencyItems;
	EmergencyItems = CreateArray(50);
	var EID;
	EID = 0;
	
	while(z != 0){
		z = GetComponent(,y,,Prod);
		Amt = GetComponent(z, , , Prod);
		if(z != 0){
			if(z == DUMM) z = Mat;
			DebugLog("Attempting to remove %v (id %v)", GetName(,z), z);
			Spec = FindObjects(Find_ID(z), Find_Container(this()));
			for(var i = 0; i < Amt; i++){
				if(ContentsCount(z, this()) > 0){
					EmergencyItems[EID] = z;
					EID++;
					RemoveObject(Spec[i]);
				}else{
						DebugLog("Forging Interrupted: Missing Items");
						Message("$GatherFail$",User);
						Sound("CommandFailure", false, User);
						for(var i = 0; EmergencyItems[i] != 0; i++){
							CreateContents(EmergencyItems[i], this(), 1);
						}
						return(0);
				}
			}
		}
		y++;
	}



    Forging = true;
	SetController(-1, User);
	SetObjectStatus(2, User, false);
	
	var Mass;
	var Speed;
	var Power;
	for(var i = 0; i < GetLength(LocalN("ForgeableMass", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Mass = LocalN("ForgeableMass", FindObject2(Find_ID(FALW)))[i];
	}
		for(var i = 0; i < GetLength(LocalN("ForgeableSpeed", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Speed = LocalN("ForgeableSpeed", FindObject2(Find_ID(FALW)))[i];
	}
	
		for(var i = 0; i < GetLength(LocalN("ForgeablePower", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Power = LocalN("ForgeablePower", FindObject2(Find_ID(FALW)))[i];
	}
	
	ForgeTimer = GetMass(,Prod) * (Mass + Power + (Speed/2));
	SetAction("Forging");
	Worker = User;
	CloseMenu(Contained());
	CloseEntrance();
}

protected func ContainedUp(pCaller) 
{
  [$Production$|Image=CXCN]
  if(!Forging && !Producing)
  return(ContextForge(pCaller));
}

func Forgefx(){
	if(Forging){
	if (Random(6 * 7)){
		Smoke(+16,-20,8);
		Sound("AnvilWork*");
	}
	if (Random(8*7)) Smoke(10,-24,5+Random(3));
	
	if(GetEnergy() < 1 && FindObject2(Find_ID(ENRG))){
		DebugLog("Forging Interrupted: No Power");
		FreeWork();
		Reset();
		Message("$ForgeFail$", this());
		SetAction("Idle");
	}
	
/* 		for(var i = 0; i < ObjectCount2(Find_OCF(OCF_CrewMember), Find_Container(this())); i++){
		if(FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this()))[i] != Worker){
			Exit(FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this()))[i], -8, 25);
		}
		} */
	
	}

	if(GetActTime() > ForgeTimer && Forging && GetAction() eq "Forging"){
		SetAction("Idle");
		FinishWork();
	}
	
}

func Incineration(){
		FreeWork();
		Reset();
}