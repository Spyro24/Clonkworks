/* --- Array Mathematik --- */

#strict 2

//Alle von ST-DDT!!!

global func ArrayMathAddArray(& anArray, array anArrayAdd)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (GetLength(anArray) != GetLength(anArrayAdd))
		return -1;
	if (GetLength(anArray) == 0)
		return GetLength(anArray);
	for (var i = 0; i < GetLength(anArray); i++)
		if ((IsNumeric(anArray[i])) && (IsNumeric(anArrayAdd[i])))
			anArray[i] += anArrayAdd[i];
	return GetLength(anArray);
}

global func ArrayMathAddInt(& anArray, int dAdd)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (GetLength(anArray) == 0)
		return GetLength(anArray);
	for (var i = 0; i < GetLength(anArray); i++)
		if (IsNumeric(anArray[i]))
			anArray[i] += dAdd;
	return GetLength(anArray);
}

global func ArrayMathSubArray(& anArray, array anArraySub)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (GetLength(anArray) != GetLength(anArraySub))
		return -1;
	if (GetLength(anArray) == 0)
		return GetLength(anArray);
	for (var i = 0; i < GetLength(anArray); i++)
		if ((IsNumeric(anArray[i])) && (IsNumeric(anArraySub[i])))
			anArray[i] -= anArraySub[i];
	return GetLength(anArray);
}

global func ArrayMathSubInt(& anArray, int dSub)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (GetLength(anArray) == 0)
		return GetLength(anArray);
	for (var i = 0; i < GetLength(anArray); i++)
		if (IsNumeric(anArray[i]))
			anArray[i] -= dSub;
	return GetLength(anArray);
}

global func ArrayMathMulArray(& anArray, array anArrayMul)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (GetLength(anArray) != GetLength(anArrayMul))
		return -1;
	if (GetLength(anArray) == 0)
		return GetLength(anArray);
	for (var i = 0; i < GetLength(anArray); i++)
		if ((IsNumeric(anArray[i])) && (IsNumeric(anArrayMul[i])))
			anArray[i] *= anArrayMul[i];
	return GetLength(anArray);
}

global func ArrayMathMulInt(& anArray, int dMul)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (GetLength(anArray) == 0)
		return GetLength(anArray);
	for (var i = 0; i < GetLength(anArray); i++)
		if (IsNumeric(anArray[i]))
			anArray[i] *= dMul;
	return GetLength(anArray);
}

global func ArrayMathDivArray(& anArray, array anArrayDiv)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (GetLength(anArray) != GetLength(anArrayDiv))
		return -1;
	if (GetLength(anArray) == 0)
		return GetLength(anArray);
	for (var i = 0; i < GetLength(anArray); i++)
		if ((GetType(anArray[i]) == C4V_Int) && (GetType(anArrayDiv[i]) == C4V_Int) && (anArrayDiv[i] != 0))
			anArray[i] /= anArrayDiv[i];
	return GetLength(anArray);
}

global func ArrayMathDivInt(& anArray, int dDiv)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (!dDiv)
		return -1;
	if (GetLength(anArray) == 0)
		return GetLength(anArray);
	for (var i = 0; i < GetLength(anArray); i++)
		if (IsNumeric(anArray[i]))
			anArray[i] /= dDiv;
	return GetLength(anArray);
}

global func ArrayMathModArray(& anArray, array anArrayMod)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (GetLength(anArray) != GetLength(anArrayMod))
		return -1;
	if (GetLength(anArray) == 0)
		return GetLength(anArray);
	for (var i = 0; i < GetLength(anArray); i++)
		if ((IsNumeric(anArray[i])) && (IsNumeric(anArrayMod[i])) && (anArrayMod[i] != 0))
			anArray[i] %= anArrayMod[i];
	return GetLength(anArray);
}

global func ArrayMathModInt(& anArray, int dMod)
{
	if (GetType(anArray) != C4V_Array)
		return -1;
	if (!dMod)
		return -1;
	if (GetLength(anArray) == 0)
		return GetLength(anArray);
	for (var i = 0; i < GetLength(anArray); i++)
		if (IsNumeric(anArray[i]))
			anArray[i] %= dMod;
	return GetLength(anArray);
}

global func ArraySum(array danArray)
{
	var dResult = 0;
	for (var dValue in danArray) 
		dResult += dValue;
	return dResult;
}

global func IsNumeric(xValue)
{
	return (xValue == 0) || (GetType(xValue) == C4V_Int);
} 