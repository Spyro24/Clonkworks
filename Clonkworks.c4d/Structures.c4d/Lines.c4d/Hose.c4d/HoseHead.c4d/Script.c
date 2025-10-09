/*-- Neues Script --*/

#strict 3

local IsOn;

func Initialize() {
  IsOn = false;
  SetAction("Aim");
  SetPhase(1);
  return(1);
}

public func IsHoseOn(){ return(IsOn); }

public func Spew(int Amount, string Mat, int minPower, int maxPower){
	var angle;
	
	if(Contained()) SetDir(GetDir(Contained()));
	
	 if(GetPhase() == 2){ //Aim Horizontally
		 angle = -25;
		 if(GetDir() == DIR_Left) angle = 205;
	 }
	 
	 if(GetPhase() == 1){ //Aim Vertically
		 angle = -45;
		 if(GetDir() == DIR_Left) angle = -135;
	 }
	 
	 if(GetPhase() == 0) angle = -90; //aim up
	 
	 for(var i = 0; i < Amount; i++){
		 CastPXSX(Mat, 1, RandomX(minPower,maxPower),0,-3,angle);
	 }
}

protected func Activate(){
	[$Togl$|Image=L_RL]
	var line;
	var ovrlp = FindObject2(Find_NoContainer(), Find_OCF(OCF_LineConstruct), Find_AtPoint());
	
	for(var i in FindObjects(Find_Action("Connect"), Find_Func("isLine"))){
		if(GetActionTarget(1, i) == this() && GetActionTarget(0, i) == ovrlp){
			line = i;
			break;
		}
	}
	
	if(!line) return(Toggle());
	
	Sound("Connect");
	RemoveObject(line);
	CreateObject(FNKT);
	RemoveObject(this());
	
	return(1);
}

func Toggle(){
	Sound("Click");
	if(IsOn) IsOn = false;
	else IsOn = true;
	return(1);
}

public func ContextAimUp(){
	[$Aim1$]
	Sound("Connect");
	SetPhase(0);
}

public func ContextAimVert(){
	[$Aim2$]
	Sound("Connect");
	SetPhase(1);
}

public func ContextAimHori(){
	[$Aim3$]
	Sound("Connect");
	SetPhase(2);
}

public func RequiresLine(){ return(true); } //line attached to hose must be disconnected via the hose!