/*-- Limeox' Funktionen --*/

#strict 2

static const SIM_Precision = 1000;
static const SIM_StepPrecision = 2;

//Wie SimFlight außer, dass SimFlight2 auch SolidMasks berücksichtigt(großer Vorteil)
global func SimFlight2(int &iX, int &iY, int &iXDir, int &iYDir, int iPrecision)
{
	var iPX, iPY;
	//An Präzision anpassen
	iX *= SIM_Precision;
	iY *= SIM_Precision;
	if (!iPrecision)	iPrecision = 10;

	iXDir = iXDir * SIM_Precision / iPrecision;
	iYDir = iYDir * SIM_Precision / iPrecision;

	//Noch im Bild?
	while (Inside(GetX() + iX / SIM_Precision, 0, LandscapeWidth()) && GetY() + iY / SIM_Precision < LandscapeHeight())
	{
		//Erste Runde nicht prüfen
		if (iPX || iPY)
		{
			//Winkel und Abstand berechnen
			var iDistance = Distance(iPX / SIM_Precision, iPY / SIM_Precision, iX / SIM_Precision, iY / SIM_Precision),
			iAngle = Angle(iPX, iPY, iX, iY);

			//Weg abtasten
			for (var i = 0; i < iDistance; i += SIM_StepPrecision)
			{
				if (GBackSolid(iX / SIM_Precision + Sin(iAngle, i), iY / SIM_Precision - Cos(iAngle, i)))
				{
					iX = iX/SIM_Precision + Sin(iAngle, i);
					iY = iY/SIM_Precision - Cos(iAngle, i);
					iXDir = iXDir * iPrecision / SIM_Precision;
					iYDir = iYDir * iPrecision / SIM_Precision;

					return true;
				}
			}

			//Schwerkraft
			iYDir += 2 * GetGravity() * SIM_Precision / 1000;
		}

		//Und weiter
		iPX = iX;
		iPY = iY;
		iX += iXDir;
		iY += iYDir;
	}

	//War wohl nix
	return false;
}

//Überprüft ob die angegebene Zahl eine Primzahl ist
global func IsPrime(int iInteger)
{
	for(var i = 2; i <= Sqrt(iInteger); i++)
	{
		if(!(iInteger % i))	return false;
	}

	return true;
}

//Setzt die Größe des angegebenen Objektes auf die angegebenen Pixel
global func SetSize(int iWidth, int iHeight, object pObj)
{
	return SetObjDrawTransform(1000 * iWidth / GetDefWidth(GetID(pObj)), 0, 0, 0, 1000 * iHeight / GetDefHeight(GetID(pObj)), 0, pObj);
}

//Erzeugt ein Array aus IDs
global func FindDefinitions()
{
  //Mal alles zusammensuchen
  var aDef = [], i, id;
  while(id = GetDefinition(i))
    aDef[i++] = id;

  //Parameter durchgehen
  var par = i = 0;
  while(par = Par(i++))
  {
    //Fail
    if (GetType(par) != C4V_Array)
      continue;

    var new_def = [];

    //Find_Func
    if (par[0] == C4FO_Func)
    {
      for (var id in aDef)
        if (DefinitionCall(id, par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9]))
          new_def[GetLength(new_def)] = id;
      aDef = new_def;
    }

    //Find_Category
    if (par[0] == C4FO_Category)
    {
      for (var id in aDef)
        if (GetDefCoreVal("Category", "DefCore", id) & par[1])
          new_def[GetLength(new_def)] = id;
      aDef = new_def;
    }

    //Find_ID... whatever
    if (par[0] == C4FO_ID)
      aDef = [par[1]];

    //Find_Or
    if (par[0] == C4FO_Or)
    {
      for (var k = 1; k <= 9; k++)
        if (par[k])
          for (var id in FindDefinitions(par[k]))
            if (GetIndexOf(id, new_def) == -1)
              new_def[GetLength(new_def)] = id;
      aDef = new_def;
    }

    //Find_And
    if (par[0] == C4FO_And)
    {
      var extra = FindDefinitions(par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9]);
      for (var id in FindDefinitions())
        if (GetIndexOf(id, aDef) != -1 && GetIndexOf(id, extra) != -1)
          new_def[GetLength(new_def)] = id;
      aDef = new_def;
    }
  }

  //Nochmal durchgehen, für Find_ExcludeID und Find_Not
  var par = i = 0;
  while(par = Par(i++))
  {
    //Fail
    if (GetType(par) != C4V_Array)
      continue;

    var new_def = [];

    //Find_Not
    if (par[0] == C4FO_Not)
    {
      var extra = FindDefinitions(par[1]);
      for (var id in aDef)
        if (GetIndexOf(id, extra) == -1)
          new_def[GetLength(new_def)] = id;
      aDef = new_def;
    }

    //Find_ExcludeID
    if (par[0] == C4FO_Exclude)
    {
      for (var id in aDef)
        if (id != par[1])
          new_def[GetLength(new_def)] = id;
      aDef = new_def;
    }

  }

  return aDef;
}

global func Find_ExcludeID(id id)
{
  if (!id)
    id = GetID(this);
  return [C4FO_Exclude, id];
}

//Sucht einen String in einem anderen und gibt die Anfangsposition zurück
global func FindString(string szString, string szFind)
{
  for (var i; i < GetLength(szString); i++)
  {
    if (GetChar(szString,i) == GetChar(szFind))
    {
      var f = true;
      for (var j = 0; j < GetLength(szFind); j++)  
        if (GetChar(szString, i + j) != GetChar(szFind, j))
          f = false;
        if (f)
          return i;
    }
  }  
  return -1;
}

//Ersetzt einen String durch einen anderen
global func ReplaceString(string &szString, string szFind, string szReplace)
{
  var start = FindString(szString, szFind);
  if (start == -1)
    return;
  //String teilen
  var part1, part2, part3;
  SplitString(szString, part1, part2, start);
  SplitString(szString, part2, part3, start + GetLength(szFind));
  szString = Format("%s%s%s", part1, szReplace, part3);
  return szString;
}

//Teilt einen String in 2 Teile
global func SplitString(string szString, string & part1, string & part2, int index)
{
  part1 = "";
  part2 = "";
  for (var i; i < GetLength(szString); i++)
  {
    if (i < index)
      part1 = Format("%s%c", part1, GetChar(szString, i));
    else
      part2 = Format("%s%c", part2, GetChar(szString, i));
  }
  return true;
}
