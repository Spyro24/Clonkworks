/*-- Neues Script --*/

#strict
#include CANN

func Initialize() {
  SetAction("Aim");
  SetPhase(10);
  return(1);
}

public func Fire(bool fAuto)
{
  var iX = ((GetPhase()+GetPhase()/2)-8)*(GetDir()*2-1);
  var iY = (GetPhase()*2)-40;
  var iAngle = BoundBy(GetPhase()*5-6,0,90);
  var iXDir = Sin(iAngle,32*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-20)+GetPhase()-6;
	
	CreateParticle("PSpark",iX+iXDir,iY+iYDir,0,0,250,RGBa(255,0,0));
	CreateParticle("PSpark",iX+iXDir*2,iY+iYDir*2,0,0,200,RGBa(255,0,0));
	CreateParticle("PSpark",iX+(iXDir*3),iY+(iYDir*3),0,0,180,RGBa(255,0,0));
	CreateParticle("PSpark",iX+(iXDir*4),iY+(iYDir*4),0,0,150,RGBa(255,0,0));
 CreateParticle("PSpark",iX,iY,iXDir+RandomX(-5,5),iYDir+RandomX(-5,5),75,RGBa(255,255,255));
  return(1);
}

func Recollect(){
	Fire();
}