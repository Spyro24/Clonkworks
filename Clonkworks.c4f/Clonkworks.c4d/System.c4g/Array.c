/* --- Array Funktionen --- */


//Alle von ST-DDT!!!




#strict 2

//Gibt das erste Vorkommen an
global func ArrayDataIndex(array anArray, xnSearch, int dnStart)
{
	if (!GetLength(anArray))
		return -1;
	if (dnStart >= GetLength(anArray))
		return -1;
	for (var i = dnStart; i < GetLength(anArray); i++)
		if (anArray[i] == xnSearch)
			return i;
	return -1;
}

//Gibt die Anzahl der Vorkommen an
global func ArrayDataCount(array anArray, xnSearch)
{
	if (!GetLength(anArray))
		return -1;
	var dtAnz = 0;
	for (var xTemp in anArray) 
		if (xTemp == xnSearch)
			dtAnz++;
	return dtAnz;
}

//Hängt einen Wert an den Array an
global func ArrayAdd(& anArray, xnNew, bool bNoDuplicates)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (bNoDuplicates)
	{
		var dTemp = ArrayDataIndex(anArray, xnNew);
		if (dTemp != -1)
			return dTemp;
	}
	anArray[GetLength(anArray)] = xnNew;
	return GetLength(anArray) - 1;
}

//Sortiert einen Wert an einer Bestimmten Position ein
global func ArraySortAdd(& anArray, xnNew, string sSortFunc, bool bReverse)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (!GetLength(anArray))
		return ArrayAdd(anArray, xnNew);
	var i = -1;
	while (++i < GetLength(anArray))
		if (anArray[i] != xnNew)
			if (eval(Format("Sort_%s(%v,%v)", sSortFunc, anArray[i], xnNew)) == bReverse)
				return ArrayInsert(anArray, xnNew, i);
	return ArrayAdd(anArray, xnNew);
}

//Fügt einen Wert an einer Position ein
global func ArrayInsert(& anArray, xnNew, int dPosition)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (dPosition < 0)
		return -1;
	if (dPosition >= GetLength(anArray))
	{
		anArray[dPosition] = xnNew;
		return dPosition;
	}
	var dTemp = GetLength(anArray);
	while (--dTemp >= dPosition)
	{
		anArray[dTemp + 1] = anArray[dTemp];
	}
	anArray[dPosition] = xnNew;
	return dPosition;
}

//Löscht einen Index (andere Objekte rutschen nach)
global func ArrayDeleteIndex(& anArray, int dPosition)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (!Inside(dPosition, 0, GetLength(anArray) - 1))
		return -1;
	var dTemp = dPosition - 1;
	while (++dTemp < GetLength(anArray) - 1)
	{
		anArray[dTemp] = anArray[dTemp + 1];
	}
	SetLength(anArray, GetLength(anArray) - 1);
	return GetLength(anArray);
}

//Löscht das erste/alle Vorkommen des Eintrages
global func ArrayDeleteEntry(& anArray, xnEntry, bool bDuplicates)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (!GetLength(anArray))
		return 0;
	var dTemp, dAnz = 0;
	while (((dTemp = ArrayDataIndex(anArray, xnEntry)) != -1) && (dAnz < 20))
	{
		ArrayDeleteIndex(anArray, dTemp);
		if (!bDuplicates)
			return dTemp;
		dAnz++;
	}
	return dAnz;
}

//Verschmiltz zwei Arrays
global func ArrayAddArray(& anArray, array anArray2, bool bNoDuplicates)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (GetType(anArray2) != C4V_Array)
		return -1;
	if (!GetLength(anArray2))
		return GetLength(anArray);
	for (var xTemp in anArray2) 
		ArrayAdd(anArray, xTemp, bNoDuplicates);
	return GetLength(anArray);
}

//Löscht alle einträge von Array2 in Array1
global func ArrayRemoveArrayEntries(& anArray, array anArray2)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (GetType(anArray2) != C4V_Array)
		return -1;
	if (!GetLength(anArray2))
		return GetLength(anArray);
	for (var xTemp in anArray2) 
		ArrayDeleteEntry(anArray, xTemp, 1);
	return GetLength(anArray);
}

//Löscht alle doppelten Einträge
global func ArrayRemoveDuplicates(& anArray)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	var atArray = anArray;
	anArray = [];
	ArrayAddArray(anArray, atArray, 1);
	return GetLength(anArray);
}

//Sortiert Arrays
global func ArraySort(& anArray, string sSortFunc, bool bReverse)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (!GetLength(anArray))
		return 0;
	var i = -1;
	while (++i < GetLength(anArray) - 1)
		if (anArray[i] != anArray[i + 1])
			if (eval(Format("Sort_%s(%v,%v)", sSortFunc, anArray[i], anArray[i + 1])) == bReverse)
			{
				ArraySortAdd(anArray, anArray[i + 1], sSortFunc, bReverse);
				ArrayDeleteIndex(anArray, i + 2);
			}
	return GetLength(anArray);
}

//had to add this function myself because it does exist - kodenith
global func GetDefinitionIndex(idDef){
	var num = 0;
	var ix = GetDefinition(num,0);
	while(ix != 0){
		ix = GetDefinition(num,0);
		if(ix == idDef) return(num);
		num++;
	}
	return(0);
}

global func Sort_IDOrder(id idDef, id idDef2)
{
	return Sort_NumericOrder(GetDefinitionIndex(idDef), GetDefinitionIndex(idDef2));
}

global func Sort_NumericOrder(int dInt, int dInt2)
{
	return dInt < dInt2;
}

//Filtert den Arrayinhalt
global func ArrayFilter(& anArray, string sFilterFunc, xnFilterPara, bool bInvert)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (!GetLength(anArray))
		return 0;
	var atArray = [];
	for (var xtEntry in anArray) 
		if (GetType(xtEntry) == C4V_C4Object)
		{
			if (eval(Format("Filter_%s(Object(%d),%v)", sFilterFunc, ObjectNumber(xtEntry), xnFilterPara)) != bInvert)
				ArrayAdd(atArray, xtEntry);
		}
		else
		{
			if (eval(Format("Filter_%s(%v,%v)", sFilterFunc, xtEntry, xnFilterPara)) != bInvert)
				ArrayAdd(atArray, xtEntry);
		}
	anArray = atArray;
	return GetLength(anArray);
}

global func Filter_Type(xnEntry, int xnPara)
{
	return GetType(xnEntry)==xnPara;
}

//Geschützer Array Zugriff
global func ArrayData(array anArray, int dnIndex, xExeption)
{
	if (Inside(dnIndex, 0, GetLength(anArray) - 1))
		return anArray[dnIndex];
	return xExeption;
}

//Zahlen im Array anpassen
global func ArrayBoundBy(& anArray, int dnMin, int dnMax, bool bRemoveDuplicates)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	for (var i = 0; i < GetLength(anArray); i++)
		if ((GetType(anArray[i]) == C4V_Int) || (anArray[i] == 0))
			anArray[i] = BoundBy(anArray[i], dnMin, dnMax);
	if (bRemoveDuplicates)
		ArrayRemoveDuplicates(anArray);
	return GetLength(anArray);
}