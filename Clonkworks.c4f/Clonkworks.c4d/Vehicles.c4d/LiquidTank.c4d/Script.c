#strict 2

local LiquidType; //type of liquid
local Amount; //Max 2000 pixels

func Initialize() {
	LiquidType = 0;
	Amount = 0;
  SetAction("FillUp");
  SetPhase(0);
  return(1);
}

func MaxAmount(){ return(2000); }
func AmountDevision(){ return(100);}

func LiquidCheck(){
	if(!LiquidType) Amount = 0;
	if(Material(LiquidType) == -1) Amount = 0;
	if(Amount > MaxAmount()) Amount = MaxAmount();
	if(Amount < 0) Amount = 0;
	if(Amount == 0) LiquidType = 0;
	
	SetPhase(Min(Amount / AmountDevision(),19));
	
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