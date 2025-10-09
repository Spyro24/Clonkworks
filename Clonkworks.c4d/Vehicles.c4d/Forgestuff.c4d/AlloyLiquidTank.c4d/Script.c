#strict 2
#include LQTK
#include DUMM

func FRGUpdate(){
	//SetColorDw(Color, this());
	SetMass(25*Mass, this());
	SetGraphics("Liquid",this(),GetID(),2,GFXOV_MODE_ExtraGraphics);
	SetGraphics(0,this(),GetID(),1,GFXOV_MODE_Base);
	SetClrModulation(Color,this(),1);
	AddEffect("Liquidshow",this(),20,0,this());
}

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
	SetClrModulation(RGBa(r,g,b), this(),2);
	}
}


protected func FxLiquidshowInfo(){
	return Format("{{BARL}} (%d px)",MaxAmount());
}

func MaxAmount(){ return(Power*1000); }
func AmountDevision(){ return(MaxAmount()/20);}
