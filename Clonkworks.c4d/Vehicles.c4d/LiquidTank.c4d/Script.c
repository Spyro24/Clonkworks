#strict 2

local LiquidType; //type of liquid
local Amount; //Max 4000 pixels

func Initialize() {
	LiquidType = 0;
	Amount = 0;
  SetAction("FillUp");
  SetPhase(0);
  return(1);
}

public func ExitWorkshop(){
	if(GetCon() < 100) return(false);
	return(true);
}

func MaxAmount(){ return(3000); }
func AmountDevision(){ return(150);}

func LiquidCheck(){
	if(!LiquidType) Amount = 0;
	if(Material(LiquidType) == -1) Amount = 0;
	if(Amount > MaxAmount()) Amount = MaxAmount();
	if(Amount < 0) Amount = 0;
	if(Amount == 0) LiquidType = 0;
	
	SetPhase(Min(Amount / AmountDevision(),19));
	SetMass(250+(Amount/3));
	
	if(LiquidType && Amount != 0){
	var r, g, b;
	r = GetMaterialVal("Color", "Material", Material(LiquidType),0);
	g = GetMaterialVal("Color", "Material", Material(LiquidType),1);
	b = GetMaterialVal("Color", "Material", Material(LiquidType),2);
	SetColorDw(RGBa(r,g,b), this());
	}
}

public func AcceptTransfer(){
	if(Amount < MaxAmount()) return(1);
}

//inserting and removing materials inside
public func InsertLiquidPx(string Type, int iAmount){
	if(iAmount < 0) return(0);
	if(Type == "Sky") return(0);
	if(!LiquidType){
		LiquidType = Type;
	}
	
	for(var i = 0; i < iAmount; i++){
		if(Type != LiquidType || Amount >= MaxAmount()){
			InsertMaterial(Material(Type));
		}else{
			Amount++;
		}
	}
	
	return(Amount);
}

public func DepositLiquidPx(int iAmount){
	for(var i = 0; i < iAmount; i++){
		if(Amount <= 0) return(i);
		InsertMaterial(Material(LiquidType));
		Amount--;
	}
}

public func SetLiquidType(string Type){
	LiquidType= Type;
}

public func SetLiquidAmount(int am){
	Amount = am;
	if(Amount > MaxAmount()) Amount = MaxAmount();
	if(Amount < 0) Amount = 0;
}

public func GetLiquidType(){ return(LiquidType); }
public func GetLiquidAmount(){ return(Amount); }

public func IsLiquidStorage(){ return(1); }

func Destruction(){
	if(Amount && LiquidType){
		for(var i = 0; i < Amount; i++){
			InsertMaterial(Material(LiquidType),0,0,RandomX(-100,100),RandomX(-100,100));
		}
	}
}

public func IsAdvancedProduct(){ return(1); }

public func ControlThrow(pByObj){
	[$Fill$|Image=L_FL]
	
	//controller must be holding this
	if(pByObj->GetAction() != "Push") return(0);
	if(GetActionTarget(0,pByObj) != this()) return(0);
	
	//check if player is holding barrel
	var Count = ContentsCount(,pByObj);
	var barrel;
	for(var i = 0; i < Count; i++){
		if(Contents(i,pByObj)->~BarrelMaxFill()){
			barrel = Contents(i,pByObj);
			break;
		}
	}
	
	if(GetID(barrel) == BARL) {
		//check if has enough liquid
		if(Amount < barrel->BarrelMaxFill()){
			Message("$FillEmpty$",this(),LiquidType);
			Sound("CommandFailure1");
			return(1);
		}
		//empty? fill it up!
		if(Amount >= barrel->BarrelMaxFill()){
			if(LiquidType == "Water") ChangeDef(WBRL,barrel);
			else if(LiquidType == "Acid") ChangeDef(ABRL,barrel);
			else if(LiquidType == "Oil") ChangeDef(OBRL,barrel);
			else if(LiquidType == "Lava" || LiquidType == "DuroLava") ChangeDef(LBRL,barrel);
			else{
				Message("$FillFail$",this(),LiquidType);
				Sound("CommandFailure1");
				return(1);
			}
			
			LocalN("iFillLevel", barrel) = barrel->BarrelMaxFill();
			Amount -= barrel->BarrelMaxFill();
			Sound("Splash2");
		}
		
		return(1);
	}
	
	//dispense liquid
	if(!barrel){
		Message("$FillNoBarrel$",this(),LiquidType);
		Sound("CommandFailure1");
		return(1);
	}
	
	var mat, cnt;
	cnt = barrel->GetAmount();
	mat = barrel->BarrelMaterialName();
	if(!cnt || !mat) return(1);
	
	barrel->BarrelDoFill(-cnt);
	ChangeDef(BARL, barrel);
	InsertLiquidPx(mat, cnt);
	Sound("Splash1");
	
	return(1);
}

public func ContextFill(pByObj){
	[$Fill$|Image=L_FL]
	SetCommand(pByObj, "Grab", this());
	AppendCommand(pByObj, "Call", this(), pByObj, 0, 0,,"ControlThrow");
}

public func ControlDig(pByObj){
	[$Release$|Image=L_RL]
	if(Amount != 0){
		Sound("AirLock2");
		DepositLiquidPx(Amount);
	}
}

public func ContextRelease(pByObj){
	[$Release$|Image=L_RL|Condition=HasLiquid]
	SetCommand(pByObj, "Grab", this());
	AppendCommand(pByObj, "Call", this(), pByObj, 0, 0,,"ControlDig");
}

private func MaxContents() { return(0); }
func RejectContents(){ return(1); }
func HasLiquid(){ return(Amount > 0); }
func IsFull(){ return(Amount >= MaxAmount()); }

public func ALKConnectType(){
	return([HOSE]);
}

// leftover from wagon
protected func ControlLeft(object clonk)
{
  [$TxtLeft$|Image=STMS:1]
  var pEngine = FindObject(0,0,0,0,0,OCF_Grab,"Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlLeft(clonk);
  return(1);
}

protected func ControlRight(object clonk)
{
  var pEngine = FindObject(0,0,0,0,0,OCF_Grab,"Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlRight(clonk);
  return(1);
}

protected func ControlLeftDouble(object clonk)
{
  var pEngine = FindObject(0, 0,0,0,0, OCF_Grab, "Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlLeftDouble(clonk);
  return(1);
}

protected func ControlRightDouble(object clonk)
{
  var pEngine = FindObject(0, 0,0,0,0, OCF_Grab, "Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlRightDouble(clonk);
  return(1);
}

protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  var pEngine = FindObject(0, 0,0,0,0, OCF_Grab, "Push",this());
  if(pEngine) pEngine->~ControlUpdate(clonk, comdir, dig, throw);
}

protected func ControlDownSingle(object clonk)
{
  var pEngine = FindObject(0, 0,0,0,0, OCF_Grab, "Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlDownSingle(clonk);
  return(1);
}
public func GetResearchBase() { return(PUMP); }