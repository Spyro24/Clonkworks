/*-- Neues Script --*/

#strict 2
#include CANN

local Steam;

func Initialize() {
  SetAction("Aim");
  SetPhase(15);
  Steam = 700;
  return(1);
}

public func Fire(bool fAuto)
{
	if(GetAction() != "Ready"){
		Sound("Click");
		var CalcedTime = ((700 - Steam) * 5) / 38;
		if(CalcedTime == 0) CalcedTime = 1;
		Message("$TxtWait$",this(),CalcedTime);
		return(0);
	}
	
  var iX = ((GetPhase()+GetPhase()/2)-8)*(GetDir()*2-1);
  if(iX < 0) iX = 0;
  var iY = (GetPhase()*2)-40;
  var iAngle = BoundBy(GetPhase()*5-6,0,90);
  var iXDir = Sin(iAngle,32*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-20)+GetPhase()-6;
	
	// test markers
	/* CreateParticle("PSpark",iX+iXDir,iY+iYDir,0,0,250,RGBa(255,0,0));
	CreateParticle("PSpark",iX+iXDir*2,iY+iYDir*2,0,0,275,RGBa(255,0,0));
	CreateParticle("PSpark",iX+(iXDir*3),iY+(iYDir*3),0,0,300,RGBa(255,0,0));
	CreateParticle("PSpark",iX+(iXDir*4),iY+(iYDir*4),0,0,350,RGBa(255,0,0));
	CreateParticle("PSpark",iX+(iXDir*5),iY+(iYDir*5),0,0,375,RGBa(255,0,0));
	CreateParticle("PSpark",iX+(iXDir*6),iY+(iYDir*6),0,0,400,RGBa(255,0,0)); */

	var BlownObjects = FindObjects(Find_Or(Find_Category(C4D_Living), Find_Category(C4D_Vehicle), Find_Category(C4D_Object)),Find_Distance(18, iX+(iXDir*1), iY+(iYDir*1)), Find_Exclude(this()), Find_NoContainer());
	var DoFinding = true;
	
	for(var i = 2; i <= 6; i++){
		if(!GBackSolidCircle(iX+(iXDir*i), iY+(iYDir*i), (i+3) * 5) && DoFinding){
		ArrayAddArray(BlownObjects, FindObjects(Find_Or(Find_Category(C4D_Living), Find_Category(C4D_Vehicle), Find_Category(C4D_Object)),Find_Distance((i+3) * 5, iX+(iXDir*i), iY+(iYDir*i)), Find_Exclude(this()), Find_NoContainer()), true);
		}else{
		DoFinding = false;
		}
		
		if(!DoFinding) break;
	}

	for(var obj in BlownObjects){
		Fling(obj, iXDir/2, iYDir/2,,true);
	} //wowzers! all this logic no longer uses a giant line of code!
	
	Sound("SteamBlast*");
	Steam -= 700;
	if(Steam <= 0)
	SetActionKeepPhase("Aim");
	
	for(var i = 0; i < RandomX(35,38); i++){
		var fXdir, fYdir;
		fXdir = iXDir * RandomX(1,6);
		fYdir = iYDir * RandomX(1,6);
		
		   CreateParticle("PSpark",iX,iY,fXdir+RandomX(-5,5),fYdir+RandomX(-5,5),RandomX(75,200),RGBa(255,255,255, RandomX(0,255)));
	}
  return(1);
}

func Recollect(){
	if(GetCon() < 100) return(0);
  if(GetAction() == "Ready") return(0);
  if(Steam < 700){
	 Steam++;
  }else{
	  Sound("Lever1");
	  SetActionKeepPhase("Ready");
  }
}

func ControlThrow(){
	  return(Fire());
}

public func IsAdvancedProduct(){
	return(1);
}

public func Departure(){
	SetPosition(GetX(),GetY()+37);
}

public func GetResearchBase() { return(CANN); }

public func FireAt(int iX,int iY,int fAuto)
{
  var iAngle;
  // Zielwinkel
  iAngle = Angle(GetX(),GetY(),iX,iY);
  // Bei größerer Distanz höher zielen
  if(Inside(iX-GetX(),+1,+300))
     iAngle -= Abs(iX-GetX())/12;
  if(Inside(iX-GetX(),-300,-1))
     iAngle += Abs(iX-GetX())/12;
  // Zielen
  AimToAngle(iAngle);
  // Feuern
    if(!GetEffect("MouseAim",this()))
  AddEffect("MouseAim",this(),100,4,this());
  return(1);
}