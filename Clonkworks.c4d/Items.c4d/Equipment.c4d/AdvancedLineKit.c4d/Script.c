/*-- Leitungsbausatz --*/

#strict

//objects that only support custom lines should have the unused line connect type set. LineConnect=128

protected func Hit()
{
  Sound("RockHit*");
}

protected func Activate(clonk)
{
  [$TxtConnectline$]
  var obj = FindObject(0, 1,0,0,0, OCF_LineConstruct(), 0,0,NoContainer(), obj);
  if(!obj) return(Message("$TxtNoNewLine$", this()) );
  var pLine = FindObject(0, 0,0,0,0, 0, "Connect", this() );
  SetComDir(COMD_Stop(), clonk);

  if(pLine)
  {
    if(obj == GetActionTarget(0, pLine) || obj == GetActionTarget(1, pLine) )
    {
      // Leitung wieder entfernen
      Message("$TxtLineRemoval$", this(), GetName(pLine) );
      RemoveObject(pLine);
      Sound("Connect");
      return(1);
    }
    else
    {
      if(!ConnectLine(pLine, obj) ) return(1);
    }
  }
  else
  {
    LineSelection(obj, clonk);
  }

  return(1);
}

public func LineSelection(pObject,clonk){
	if(!pObject->~ALKConnectType()){
	  Sound("Error");
      return(Message("$TxtNoNewLine$", this()) );
	}
	var linelist = pObject->~ALKConnectType();
	if(GetLength(linelist) == 1){
		CreateLine(linelist[0], GetOwner(), pObject, this());
	}else{
		CreateMenu(FNKT,clonk,this(),4,"$TxtNoNewLine$");
		for(var i in linelist){
			AddMenuItem("$TxtConnectline$: %s", "LineSelectionDone",i,clonk,0,pObject,GetDesc(,i));
		}
	}
}

protected func LineSelectionDone(linetype, from){
  var obj = FindObject(0, 1,0,0,0, OCF_LineConstruct(), 0,0,NoContainer(), obj);
  if(!obj) return(Message("$TxtNoNewLine$", this()));
  if(obj != from) return(Message("$TxtNoNewLine$", this()));
  
  CreateLine(linetype, GetOwner(), from, this());
}

private func CreateLine(linetype, owner, from, to)
{
  var line = CreateObject(linetype, 0, 0, owner);
  ObjectSetAction(line, "Connect", from, to);
  Message("$TxtNewLine$", this(), GetName(line) );
  Sound("Connect");
  return(line);
}

/* Verbinden zweier Gebäude mit einer Leitung */
private func ConnectLine(line, to)
{
  var line_accept;
  if(to->~ALKConnectType()){
	  if(GetIndexOf(GetID(line), to->~ALKConnectType()) != -1){
		  line_accept = true;
	  }else{
		  line_accept = false;
	  }
  }else{
	  line_accept = false;
  }

  var t1 = GetActionTarget(0, line);
  var t2 = to;
  if(t1 == this() ) { t2 = GetActionTarget(1, line); t1 = to; }
  
  var ok = false;
    if (t1 == to && line_accept)
      ok = true;
    if (t2 == to && line_accept)
      ok = true;


  if(!ok)
  {
    Sound("Error");
    Message("$TxtNoConnectType$", this(), GetName(line), GetName(to) );
    return(0);
  }
  
  ObjectSetAction(line, "Connect", t1, t2);

  Message("$TxtConnect$", Contained(), GetName(line), GetName(to) );
  Sound("Connect");
  RemoveObject();
  return(1);
}

/* Produkteigenschaften */
public func IsAnvilProduct() { return(1); }
public func IsAdvancedProduct() { return(1); }
public func IsKit() { return(1); }

//Info menu
public func ContextInfo(pClonk){
	[$TxtLineInfo$|Image=CXIN]
	CreateMenu(CXIN,pClonk,this(),4);
	for(var i = 0; GetDefinition(i) != 0; i++){
		var j = GetDefinition(i);
		if(DefinitionCall(j,"isLine") && j != PWRL && j != DPIP && j != SPIP){
			AddMenuItem(GetName(,j),"DoLineDesc",j,pClonk,0,GetOwner(pClonk),GetDesc(,j));
		}
	}
}

public func DoLineDesc(line,owner){
	MessageWindow(GetDesc(,line),owner,CXIN,GetName(,line));
}