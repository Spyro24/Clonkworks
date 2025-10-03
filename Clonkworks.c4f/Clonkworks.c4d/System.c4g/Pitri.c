/*-- Pitris Funktionen --*/

#strict 2

//Bestandteil eines Arrays?
global func PartOf(xPart, array aArray, bool fExact)
{
	for(var i; i < GetLength(aArray); i++)
	{
			if(xPart == aArray[i])	return true;

		if(fExact && GetType(aArray[i]) == C4V_Array)
		{
			if(PartOf(xPart,aArray[i],true))	return true;
		}

		return false;
	}
}


//Prüft, ob ein String rein numerisch ist
global func IsNumerical(string szInput)
{
	for(var i; i < GetLength(szInput); i++)
	{
		if(GetChar(szInput,i) < 48 || GetChar(szInput,i) > 57)	return false;
	}

	return true;
}