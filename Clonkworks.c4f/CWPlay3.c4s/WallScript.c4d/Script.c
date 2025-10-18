/*-- Wandnachricht --*/

#strict 2

local txt_id; // Text-ID
local txt_text; // Text-string oder Array für längere Texte

static g_wallscript_read_list; // Liste der Wandtexte, die man schon gelesen hat und nun alle per Rechtsklick lesen duerfen

protected func Initialize()
  {
  SetAction("Walling");
  }

protected func ControlUp(object pPlayer)
  { [$CtrlRead$]
  return DoRead(pPlayer, false);
  }

private func DoRead(object pPlayer, bool remote)
  {
  if (!remote && !HasReadScript()) Add2ReadList();
  if (GetAction() == "ABC")
    {
    PlayerMessage(GetController(pPlayer), ["[ A ]", "[ B ]", "[ C ]"][GetPhase()], pPlayer);
    Sound("CommandFailure*", false, pPlayer);
    return true;
    }
  if (GetAction() == "Unreadable")
    {
    PlayerMessage(GetController(pPlayer), "$MsgCantRead$", pPlayer);
    Sound("CommandFailure*", false, pPlayer);
    return true;
    }
  if (GetAction() == "Engine")
    {
    PlayerMessage(GetController(pPlayer), "$MsgEngineRestart$", this);
    if (!remote)
      {
      var pEngine = FindObject2(Find_ID(STMG));
      if(GetDir(pEngine)) pEngine->ControlRightDouble(this);
      else pEngine->ControlLeftDouble(this);
      Sound("Discharge*");
      }
    return true;
    }
  if (GetAction() == "Reconnect")
    {
    PlayerMessage(GetController(pPlayer), "$MsgReconnectWindmill$", this);	
    if (!remote)
      {
      RemoveAll(PWRL);
      RemoveAll(LNKT);
      CreateObject(PWRL,50,50,-1)->SetAction("Connect", FindObject(WMIL),pPlayer);
      Sound("Connect");
      }
    return true;
    }
  return DoMsg(pPlayer, txt_text, this);
  }

public func DoMsg(object clonk, vTxt, object target)
{
  if (!CreateMenu(GetID(target), clonk, target, 0, "", 0, C4MN_Style_Dialog | C4MN_Style_EqualItemHeight)) return false;
  AddMsgText(clonk, vTxt);
  SetMenuDecoration(MD69, clonk);
  return true;
}

private func AddMsgText(object pPlayer, vTxt)
  {
  // Mögliche Formate für vTxt sind:
  // "Text"
  // [ID, "Text"]
  // ["Text1", [ID, "Text2"], ...]
  var sString, idIcon=NONE;
  if (GetType(vTxt) == C4V_Array)
    {
    var vItem = vTxt[0];
    if (GetType(vItem) == C4V_C4ID)
      {
      // Array vom Typ [C4ID, "Text"]?
      idIcon = vItem;
      sString = vTxt[1];
      // [C4ID, [...]]: Nur Icon eintragen
      if (GetType(sString) == C4V_Array)
        {
        AddMenuItem(Format("%i", idIcon), "", NONE, pPlayer, 0, 0, "", C4MN_Add_ImgTextSpec, 0, 0);
        return(AddMsgText(pPlayer, sString));
        }
      }
    else
      {
      // Ansonsten Array, das weitere Texte enthält
      for (vItem in vTxt) AddMsgText(pPlayer, vItem);
      return true;
      }
    }
  else
    sString = vTxt;
  return(AddMenuItem(sString, "", idIcon, pPlayer, 0, 0, "", C4MN_Add_ForceNoDesc, 0, 0));
  }

public func SetText(sToText)
  {
  txt_text = sToText;
  return true;
  }
  
public func UpdateText()
  {
  if (txt_id)
    {
      txt_text = GameCall("GetLangText", txt_id);
    }
  }

global func UpdateWallScriptTexts()
  {
  for (var obj in FindObjects(Find_ID(_WSC)))
    obj->UpdateText();
  return true;
  }

public func ResetReadList()
  {
  g_wallscript_read_list = 0;
  }

func ContextRead(object clonk)
  {
  [$CtrlRead$|Image=_WSC|Condition=HasReadScript]
  return DoRead(clonk, true);
  }

func HasReadScript()
  {
  if (!g_wallscript_read_list) return;
  return GetIndexOf(txt_id, g_wallscript_read_list) >= 0;
  }

func Add2ReadList()
  {
  if (!g_wallscript_read_list)
    g_wallscript_read_list = [txt_id];
  else 
    g_wallscript_read_list[GetLength(g_wallscript_read_list)] = txt_id;
  }
  
global func FxInfo_WSCInfo(object pTarget, int iIndex, int iTime)
{
  return("$Info_WSC$");
}
  
  
