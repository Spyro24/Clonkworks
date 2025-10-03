/*-- Tackleberrys Funktionen --*/

#strict 2

//Bringt die (Un)Ordnung eines Arrays durcheinander
global func ShuffleArray(array a)
{
	var aArray = a;

	for(var n = GetLength(aArray); n > 1; n--)
	{
		var k = Random(n + 1);
		var memswap = aArray[k];	//Element von aArray[k] speichern
		aArray[k] = aArray[n];		//Den Wert von aArray[k] ersetzen
		aArray[n] = memswap;		//und den gespeicherten Wert an die Stelle des Anderen setzen
	}

	return aArray;
}
