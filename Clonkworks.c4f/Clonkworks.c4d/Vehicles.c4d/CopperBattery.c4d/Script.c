/*-- Neues Script --*/

#strict

local Charge;

func Initialize(){
		SetCommand(this(), "Exit");
}

func Construction(){
	Charge = 0;
}

public func GetChargeHold(){ return(20); }

func UpdateLook(){
	var Fixed = false;
	
	var eng = GetEnergy();
	
	if(!Random(GetChargeHold()))
	if(Charge > eng){
		SetBatteryEnergy(Charge-1);
	}else if(Charge < eng){
		SetBatteryEnergy(Charge+1);
	};
	
	if(!Random(GetChargeHold()) && Charge <= 2 && Charge > 0){
		SetBatteryEnergy(Charge-1);
	}
	
	if(GetAction() ne "Charge") SetAction("Charge");
	SetPhase(Charge-1);
}

//useful functions that make working with batteries and their energy a bit easier (i blame myself for my horrid code)

func IsBattery(){ return(1); }

func DoBatteryEnergy(int Amount){
	if(Abs(Amount) > 100) return(0);
	DoEnergy(Amount);
	Charge = GetEnergy();
	return(1);
}

func SetBatteryEnergy(int Amount){
	if(Amount < 0) return(0);
	DoEnergy(-GetEnergy());
	DoEnergy(Amount);
	Charge = Amount;
	return(1);
}

public func IsAdvancedProduct(){ return(1); }