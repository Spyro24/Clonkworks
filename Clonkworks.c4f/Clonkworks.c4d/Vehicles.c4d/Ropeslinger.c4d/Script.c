/*-- Neues Script --*/

#strict
#include CANN

protected func ControlConf(int conf)
{
  if(AimStdConf(conf))
    Sound("CatapultSet");
}

public func ControlUp(object clonk)     // Zielen: hoch (klassisch)
{
  [$TxtAimup$|Image=CAN1:2]
  AimUp(clonk, 2, "ControlConf");
}

public func ControlDig(object clonk)    // Zielen: runter (klassisch)
{
  [$TxtAimdown$|Method=Classic|Image=CAN1:0]
  AimDown(clonk, 2, "ControlConf");
}

public func ControlDown()
{
  [$TxtAimdown$|Method=JumpAndRun|Image=CAN1:0] // Beschreibung fuer JnR
}

public func ControlUpdate(object clonk, int comdir) // Zielen: JnR
{
  AimUpdate(clonk, comdir, 2, "ControlConf");
}

//THIS IS A BASIC IMPLEMENTATION! THIS WILL BE FULLY SCRIPTED AND DONE TOMMOROW!
//TODO: Program the ropeslinger