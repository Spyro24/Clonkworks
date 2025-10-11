#strict
#include LORY
#include DUMM

local Color;
local Mass;
local Speed;
local Power;
local Effect;

func FRGUpdate(){
	//SetColorDw(Color, this());
	SetMass(25*Mass, this());
	SetClrModulation(Color, this());
}

private func MaxContents() { return(30*Power); }
public func GetResearchBase() { return(LORY); }