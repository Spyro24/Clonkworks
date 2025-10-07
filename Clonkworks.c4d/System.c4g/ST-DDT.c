/*-- Funktionen von ST-DDT --*/

#strict 2


//Gibt die Primfaktoren einer Zahl zurück
global func GetPrimeFactors(int dnInt, bool bIncludeOne)
{
	var dF = 1;
	var datFactors = [];
	if (bIncludeOne)
		datFactors[0] = 1;

	while (dnInt > 1)
	{
		dF++;
		if (!(dnInt % dF))
		{
			datFactors[GetLength(datFactors)] = dF;
			while (!(dnInt % dF))
				dnInt /= dF;
		}
	}

	return datFactors;
}


//Gibt die Anzahl der Primfaktoren einer Zahl zurück
global func GetPrimeFactorsCount(int dnInt, bool bIncludeOne)
{
	var dF = 1; //Faktor
	var dFC = 0; //FaktorAnzahl bisher
	var datFactors = [];

	if (bIncludeOne)
	{
		datFactors[0] = [1, dnInt];
		dFC++;
	}

	while (dnInt > 1)
	{
		dF++;
		if (!(dnInt % dF))
		{
			var dC=0; //Anzahl dieses Faktors
			while (!(dnInt % dF))
			{
				dnInt /= dF;
				dC++;
			}
			datFactors[dFC] = [dF, dC];
			dFC++;
		}
	}

	return datFactors;
}

