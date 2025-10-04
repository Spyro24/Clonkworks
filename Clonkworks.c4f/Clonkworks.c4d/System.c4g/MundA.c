/*-- Systemobjekt --*/

#strict 2


//Für die Erweiterungsfunktion von AddMsgBoardCmd
static aExtendedAddMsgBoardCmd;


//Überprüft ob die angegebene X- und Y-Geschwindigkeit sich im angegebenen Bereich befindet
global func CheckSpeed(int xDir , int yDir , int i1 , int i2 , int i3 , int i4 , object pObj)
{
	var iDir = -1;
	if(GetDir(pObj) == DIR_Right)	iDir = 0;


	if(!(Inside(xDir , GetXDir(pObj) -i1 , GetXDir(pObj) +i2)))
	{
		return false;
	}

	if(!(Inside(yDir , GetYDir(pObj) -i3 , GetYDir(pObj) +i4)))
	{
		return false;
	}


	return true;
}





//Überprüft ob die angegebene X-Geschwindigkeit sich im angegebenen Bereich befindet
global func CheckSpeedX(int xDir , int i1 , int i2 , object pObj)
{
	var iDir = -1;
	if(GetDir(pObj) == DIR_Right)	iDir = 0;


	if(!(Inside(xDir , GetXDir(pObj) -i1 , GetXDir(pObj) +i2)))
	{
		return false;
	}


	return true;
}






//Überprüft ob die angegebene Y-Geschwindigkeit sich im angegebenen Bereich befindet
global func CheckSpeedY(int yDir , int i1 , int i2 , object pObj)
{
	var iDir = -1;
	if(GetDir(pObj) == DIR_Right)	iDir = 0;


	if(!(Inside(yDir , GetYDir(pObj) -i1 , GetYDir(pObj) +i2)))
	{
		return false;
	}


	return true;
}


//Erzeugt ein Pixel des angegebenen Materials an der angegebenen Stelle
global func CreateMatPx(string sMaterial , int x , int y)
{
	var b = DrawMaterialQuad(sMaterial , x , y , x +1 , y , x , y +1 , x +1, y +1);
	return(b);
}



//Erzeugt an der angegebenen Stelle(vom Offset des Objektes ausgehend) einen Partikelkreis mit dem angegebenen Durchmesser, der angegeben Geschwindigkeit,Größe und Farbe
//string sParticleName , int iDistance , int iXPosition , int iYPosition , int iXSpeed , int iYSpeed , int iSize , int iColor1 , int iColor2 , iAngle , iMaxAngle
global func CreateParticleCircle(array aPar)
{
	var aFuncs = aPar;

	var sParticleName = aFuncs[0];
	var iDistance = aFuncs[1];
	var iXPosition = aFuncs[2];
	var iYPosition = aFuncs[3];
	var iXSpeed = aFuncs[4];
	var iYSpeed = aFuncs[5];
	var iSize = aFuncs[6];
	var iColor1 = aFuncs[7];
	var iColor2 = aFuncs[8];
	var iAngle = aFuncs[9];
	var iMaxAngle = aFuncs[10];

	if(iMaxAngle == 0)	iMaxAngle = 360;


	for(var i = iAngle; i < iMaxAngle; i++)
	{
		CreateParticle("PSpark" , AbsX(Cos(i , iDistance) + iXPosition + GetX()) , AbsY(Sin(i , iDistance) + iYPosition + GetY()) , iXSpeed , iYSpeed , iSize , RandomX(iColor1 , iColor2));
	}


	return true;
}

//Findet ein Objekt dessen Grafikshape-Bereich den angegebenen Kreis(teil) überlappt. Außerdem können folgende Kriterien angegeben werden(selbsterklärend):
//int iRadius , int iOCF , int iHostility , int iOwner , int iXSpeed , int iYSpeed , int iMinXDir , int iMaxXDir , int iMinYDir , int iMaxYDir , int iAngle , int iMaxAngle
global func FindAtCircleObject(array aFuncs)
{
	var aObj = [];

	var iRadius = aFuncs[0];
	var iOCF = aFuncs[1];
	var iHostility= aFuncs[2];
	var iOwner = aFuncs[3];
	var iXSpeed = aFuncs[4];
	var iYSpeed = aFuncs[5];
	var iMinXDir = aFuncs[6];
	var iMaxXDir = aFuncs[7];
	var iMinYDir = aFuncs[8];
	var iMaxYDir = aFuncs[9];
	var iAngle = aFuncs[10];
	var iMaxAngle = aFuncs[11];
	var iMinInside = Abs(aFuncs[12]);
	var iMaxInside = Abs(aFuncs[13]);

	if(iMaxAngle == 0)	iMaxAngle = 360;
	if(iAngle >= iMaxAngle)	
	{
		var iSave = iMaxAngle;
		iMaxAngle = iAngle;
		iAngle = iSave;
	}


	if(iRadius == 0)		return [];
	if(iMinXDir && !iMaxXDir)	return [];
	if(!iMinXDir && iMaxXDir)	return [];
	if(iMinYDir && !iMaxYDir)	return [];
	if(!iMinYDir && iMaxYDir)	return [];
	if(iMinInside && !iMaxInside)	return [];
	if(!iMinInside && iMaxInside)	return [];

	if(!iXSpeed && iMinXDir && iMaxXDir)	iXSpeed = (iMinXDir + iMaxXDir) / 2;
	if(!iYSpeed && iMinYDir && iMaxYDir)	iYSpeed = (iMinYDir + iMaxYDir) / 2;
	if(!iMinInside && !iMaxInside)	      { iMinInside = 5; iMaxInside = 5; }


	var a1 = iRadius;

	var a2 = Find_OCF(iOCF);
	if(GetLength(a2) == 0 || iOCF == 0)		a2 = [];
	var a3 = Find_Hostile(iHostility);
	if(GetLength(a3) == 0 || iHostility == -1)	a3 = [];
	var a4 = Find_Owner(iOwner);
	if(GetLength(a4) == 0 || iOwner == -1)		a4 = [];


	var a = [];
	for(var y = 1; y < 4; y++)
	{
		if(eval(Format("%v != []" , VarN(Format("a%d" , y)))))
		{
			a[y -1] = VarN(Format("a%d" , y));
		}

		else	a[GetLength(a)] = [];
	}



	var aObjects = FindObjects(Find_Distance(iRadius) , a[0] , a[1] , a[2] , Find_Exclude(this()));

	for(var obj in aObjects)
	{

		var iDistance = ObjectDistance(obj , this());

		if(!Inside(iDistance , iRadius -iMinInside , iRadius +iMaxInside))	PopElement(aObjects , GetIndexOf(obj , aObjects));
	}


	





	if(!iXSpeed && !iYSpeed && !iMinXDir && !iMaxXDir && !iMinYDir && !iMaxYDir)	aObj = JoinArrays(aObjects , aObj);


	else if(iXSpeed && iYSpeed)
	{
		if(iMinXDir && iMaxXDir && iMinYDir && iMaxYDir)
		{
			for(var obj in aObjects)
			{
				if(!CheckSpeed(iXSpeed , iYSpeed , iMinXDir , iMaxXDir , iMinYDir , iMaxYDir , obj))	PopElement(aObjects , GetIndexOf(obj , aObjects));
			}


			aObj = JoinArrays(aObjects , aObj);
		}

		if(iMinXDir && iMaxXDir && !iMinYDir && !iMaxYDir)
		{
			for(var obj in aObjects)
			{
				if(!CheckSpeedX(iXSpeed , iYSpeed , iMinXDir , iMaxXDir , obj))	PopElement(aObjects , GetIndexOf(obj , aObjects));
			}


			aObj = JoinArrays(aObjects , aObj);
		}

		if(!iMinXDir && !iMaxXDir && iMinYDir && iMaxYDir)
		{
			for(var obj in aObjects)
			{
				if(!CheckSpeedY(iXSpeed , iYSpeed , iMinYDir , iMaxYDir , obj))	PopElement(aObjects , GetIndexOf(obj , aObjects));
			}


			aObj = JoinArrays(aObjects , aObj);
		}

		if(!iMinXDir && !iMaxXDir && !iMinYDir && !iMaxYDir)
		{
			for(var obj in aObjects)
			{
				if(GetXDir(obj) != iXSpeed && GetYDir(obj) != iYSpeed)	PopElement(aObjects , GetIndexOf(obj , aObjects));
			}


			aObj = JoinArrays(aObjects , aObj);
		}
	}

	return aObj;
}


/*Startet eine Verfolgungsjagd. Parametererklärung:
  1. pObj: Verfolgendes Objekt
  2. pFollowObject: Zu verfolgendes Objekt
  3. iDistance: Distanz die höchstens zwischen den beiden Objekten "auftreten" darf
  4. iSeconds: Wie viele Sekunden die Verfolgung andauern soll(für Leute die mit Frames nicht gut zurecht kommen und auch damit
               man nicht z. B. 4320 beim Paramter "iFrames" schreiben muss, das sind 120 Sekunden).
  5. iFrames: Wie viele Frames die Verfolgung andauern soll. Sind der 4. und der 5. Parameter angegeben werden sie addiert.
  6. fPathFree: Die Verfolgung wird abgebrochen wenn ein Hindernis zwischen den beiden Objekten existiert
  7. iMinXDir: Untere Schranke für die X-Geschwindigkeit(siehe Anmerkung)
  8. iMaxXDir: Obere Schranke für die X-Geschwindigkeit(siehe Anmerkung)
  9. iMinYDir: Untere Schranke für die Y-Geschwindigkeit(siehe Anmerkung)
  10. iMaxYDir: Obere Schranke für die Y-Geschwindigkeit(siehe Anmerkung)
  11. iFactor: Die Geschwindigkeit wird mit iFactor multipliziert
*/


//Parameter: object pObj, object pFollowObject , int iDistance , int iSeconds , int iFrames , bool fPathFree , iMinXDir , iMaxXDir , iMinYDir , iMaxYDir , int iFactor
global func Follow(array aParameter)
{
	var aFuncs = aParameter;

	//Funktionslokale Variablen erstellen und deren Werte aus "aFuncs" holen
	var pObj = aFuncs[0];
	var pFollowObject = aFuncs[1];
	var iMinXDir = aFuncs[6];
	var iMaxXDir = aFuncs[7];
	var iMinYDir = aFuncs[8];
	var iMaxYDir = aFuncs[9];

	//Ist kein Faktor angegeben muss er wenigstens auf 1 sein, weil sonst xDir *iFact 0 ergeben würde
	if(aFuncs[10] == 0)	aFuncs[10] = 1;

	//Existieren das verfolgende Objekt, das ZielObjekt nicht oder steckt das verfolgende Objekt fest?
	if(!pObj || !pFollowObject || Stuck(pObj))	return false;

	//Es müssen immer beide Schranken bei den Geschwindigkeitsschranken gesetzt sein. Wenn nicht wird die Verfolgung gar nicht erst gestartet
	if(!iMinXDir && iMaxXDir)	return false;
	if(iMinXDir && !iMaxXDir)	return false;

	if(!iMinYDir && iMaxYDir)	return false;
	if(iMinYDir && !iMaxYDir)	return false;



	var iEffectNumber = AddEffect("Follow" , 0 , 20 , 1);

	EffectCall(0 , iEffectNumber , "SetVariables" , aFuncs);
}






global func FxFollowSetVariables(pTarget , iNumber , aFuncs)
{
	//Parameter "entpacken" und an die Effektvariablen übergeben
	EffectVar(0 , 0 , iNumber) = aFuncs[0];
	EffectVar(1 , 0 , iNumber) = aFuncs[1];
	EffectVar(2 , 0 , iNumber) = aFuncs[2];
	EffectVar(3 , 0 , iNumber) = aFuncs[3];
	EffectVar(4 , 0 , iNumber) = aFuncs[4];
	EffectVar(5 , 0 , iNumber) = aFuncs[5];
	EffectVar(6 , 0 , iNumber) = aFuncs[6];
	EffectVar(7 , 0 , iNumber) = aFuncs[7];
	EffectVar(8 , 0 , iNumber) = aFuncs[8];
	EffectVar(9 , 0 , iNumber) = aFuncs[9];
	EffectVar(10 , 0 , iNumber) = aFuncs[10];
}



global func FxFollowTimer(pTarget , iNumber , iEffectTime)
{
	//Effektvariablen-Werte holen und funktionslokalen Variablen zuweisen
	var pObj = EffectVar(0 , 0 , iNumber);
	var pFollowObject = EffectVar(1 , 0 , iNumber);
	var iDistance = EffectVar(2 , 0 , iNumber);
	var iSeconds = EffectVar(3 , 0 , iNumber);
	var iFrames = EffectVar(4 , 0 , iNumber);
	var fPathFree = EffectVar(5 , 0 , iNumber);
	var iMinXDir = EffectVar(6 , 0 , iNumber);
	var iMaxXDir = EffectVar(7 , 0 , iNumber);
	var iMinYDir = EffectVar(8 , 0 , iNumber);
	var iMaxYDir = EffectVar(9 , 0 , iNumber);
	var iFact = EffectVar(10 , 0 , iNumber);







	//Ist die Zeit schon um?
	if(iSeconds || iFrames)
	{
		if(iSeconds && !iFrames)
		{
			if(iEffectTime >= iSeconds *36)	return -1;
		}

		if(!iSeconds && iFrames)
		{
			if(iEffectTime >= iFrames)	return -1;
		}
	}

	if(iSeconds && iFrames)
	{
		if(iEffectTime >= ((iSeconds *36) + iFrames))	return -1;
	}












	//Ist das Ziel schon zuweit weg? Wenn ja Effekt abbrechen
	if(iDistance && ObjectDistance(pFollowObject , pObj) >= iDistance)	return -1;

	//Soll der Effekt abgebrochen werden, wenn zwischen dem Objekt und dem Ziel ein Hindernis existiert?
	if(fPathFree)
	{
		//Wenn ja, ist ein Hindernis da? Wenn ja, Effekt abbrechen
		if(!PathFree(GetX(pObj) , GetY(pObj) , GetX(pFollowObject) , GetY(pFollowObject)))	return -1;
	}















	/*----------------- Geschwindigkeit setzen -----------------*/


	var iX = GetX(pFollowObject);
	var iY = GetY(pFollowObject);
	var angle = Angle(GetX(pObj) , GetY(pObj) , iX , iY);
	var xDir = +Sin(angle , 16);
	var yDir = -Cos(angle , 16);



	//Sind Schranken für die X-Geschwindigkeit gegeben?
	if(iMinXDir && iMaxXDir)
	{
		//Ist nicht zuviel Geschwindigkeitsunterschied?
		if(CheckSpeedX(xDir *iFact , iMinXDir , iMaxXDir , pObj))	SetXDir(xDir *iFact , pObj);
	}

	//Sind keine Schranken gegeben einfach normal setzen
	else
	{
		SetXDir(xDir *iFact , pObj);
	}



	//Sind Schranken für die Y-Geschwindigkeit gegeben?
	if(iMinYDir && iMaxYDir)
	{
		//Ist nicht zuviel Geschwindigkeitsunterschied?
		if(CheckSpeedY(yDir , iMinYDir , iMaxYDir , pObj))	SetYDir(yDir *iFact , pObj);
	}

	//Sind keine Schranken gegeben einfach normal setzen
	else	SetYDir(yDir *iFact , pObj);
}


//Gibt das größte Element eines(oder mehrerer) Arrays(s) zurück, welcher nur aus Integers besteht
global func HighestElemInAr(array aArray , array bArray , array cArray , array dArray)
{
	var iHighest;

	var i;

	var a;
	var b;
	var c;
	var d;

	var iHigha;
	var iHighb;
	var iHighc;
	var iHighd;

	if(GetType(aArray) == C4V_Array)	a = aArray;
	else					a = [];

	if(GetType(bArray) == C4V_Array)	b = bArray;
	else					b = [];

	if(GetType(cArray) == C4V_Array)	c = cArray;
	else					c = [];

	if(GetType(dArray) == C4V_Array)	d = dArray;
	else					d = [];





	for(i in a)
	{
		for(var y in a)
		{
			if(y < i)		iHighest = i;
			else if(y > iHighest)	iHighest = y;
		}
	}

	iHigha = iHighest;


	iHighest = 0;


	for(i in b)
	{
		for(var y in b)
		{
			if(y < i)		iHighest = i;
			else if(y > iHighest)	iHighest = y;
		}
	}

	iHighb = iHighest;


	iHighest = 0;


	for(i in c)
	{
		for(var y in c)
		{
			if(y < i)		iHighest = i;
			else if(y > iHighest)	iHighest = y;
		}
	}

	iHighc = iHighest;


	iHighest = 0;


	for(i in d)
	{
		for(var y in d)
		{
			if(y < i)		iHighest = i;
			else if(y > iHighest)	iHighest = y;
		}
	}

	iHighd = iHighest;


	iHighest = 0;



	if(iHigha > iHighb && iHigha > iHighc && iHigha > iHighd)		return(iHigha);
	if(iHighb > iHigha && iHighb > iHighc && iHighb > iHighd)		return(iHighb);
	if(iHighc > iHigha && iHighc > iHighb && iHighc > iHighd)		return(iHighc);
	if(iHighd > iHigha && iHighd > iHighb && iHighd > iHighc)		return(iHighd);
}


//Lässt eine Aktion "einfrieren". Man kann sich zwar bewegen, aber die Phase der Aktion bleibt die selbe. Wird die Aktion durch eine andere ersetzt ist diese nicht eingefroren
global func HoldAction(object pObj)
{
	if(GetEffect("HoldAction" , pObj))	return false;

	var iEffectNumber = AddEffect("HoldAction" , pObj , 20 , 1 , 0 , 0);
	EffectCall(pObj , iEffectNumber, "SetVariables" , pObj , GetAction(pObj) , GetPhase(pObj));

	return true;
}



global func FxHoldActionSetVariables(pTarget , iNumber , pObj , sAction , iPhase)
{
	EffectVar(0 , pTarget , iNumber) = pObj;
	EffectVar(1 , pTarget , iNumber) = sAction;
	EffectVar(2 , pTarget , iNumber) = iPhase;
}


global func FxHoldActionTimer(pTarget , iNumber , iEffectTime)
{
	var pObj = EffectVar(0 , pTarget , iNumber);
	var sAction = EffectVar(1 , pTarget , iNumber);
	var iPhase = EffectVar(2 , pTarget , iNumber);

	if(GetAction(pObj) != sAction)	return -1;

	SetPhase(iPhase , pObj);
}



//Hebt die "Einfrierung" einer Aktion wieder auf
global func ContinueAction(object pObj)
{
	if(GetEffect("HoldAction" , pObj))	RemoveEffect("HoldAction" , pObj);
	else	return false;

	return true;
}




//Gibt einen Array zurück mit allen verfeindeten Spielern
global func Hostility(int iPlayer)
{
	var a = [];

	for(var i = 0; i < GetPlayerCount(); i++)
	{
		if(Hostile(iPlayer , GetPlayerByIndex(i) , true))
			a[GetLength(a)] = GetPlayerByIndex(i);
	}

	if(!GetLength(a))	return false;

	return a;
}


//Überprüft, ob die angegebenen Zahlen durcheinander Teilbar sind
global func IsDivisible(int i1 , int i2)
{
	return (!(i1 % i2));
}


//Verbindet die angegebenen Arrays miteinander und gibt den verbundenen Array zurück
global func JoinArrays(array a , array b)
{
	var aType = GetType(a);
	var bType = GetType(b);


	if(!aType == C4V_Array || !bType == C4V_Array)
	{
		if(!(aType == C4V_Array) && bType == C4V_Array)
			return(b);

		if(!(bType == C4V_Array) && aType == C4V_Array)
			return(a);
	}

	if(aType == C4V_Array && bType == C4V_Array)
	{
		for(var i in b)
		{
			a[GetLength(a)] = i;
		}

		return(a);
	}
}


//Gibt das kleinste Element eines(oder mehrerer) Arrays(s) zurück, welcher nur aus Integers besteht
global func LowestElemInAr(array a , array b , array c , array d)
{
	var i = 0;
	var y = 0;

	var lowesta;
	var lowestb;
	var lowestc;
	var lowestd;
	var Notlowest = 0;




	for(var x = 0; x < 4; x ++)
	{
		if(GetLength(a) == 0)
		{
			if(GetLength(b) > 0)							{	a = b;	b = 0;	continue;	}
			if(GetLength(b) == 0 && GetLength(c) > 0)				{	a = c;  c = 0;	continue;	}
			if(GetLength(b) == 0 && GetLength(c) == 0 && GetLength(d) > 0)		{	a = d;	d = 0;	continue;	}
		}


		if(GetLength(b) == 0)
		{
			if(GetLength(c) > 0)							{	b = c;	c = 0;	continue;	}
			if(GetLength(c) == 0 && GetLength(d) > 0)				{	b = d;  d = 0;	continue;	}
		}


		if(GetLength(c) == 0)
		{
			if(GetLength(d) > 0)							 {	c = d;	d = 0;	continue;	}
		}
	}

	if(GetLength(b) == 0)	b = 0;
	if(GetLength(c) == 0)	c = 0;
	if(GetLength(d) == 0)	d = 0;




	if(GetType(a) == C4V_Array)
	{
		for(i in a)
		{
			for(y in a)
			{
				if(y < i)
				{
					Notlowest = 1;
					continue;
				}
			}

			if(Notlowest == 0)
			{
				lowesta = i;
				break;
			}

			else	Notlowest = 0;
		}
	}



	Notlowest = 0;

	if(GetType(b) == C4V_Array)
	{
		for(i in b)
		{
			for(y in b)
			{
				if(y < i)
				{
					Notlowest = 1;
					continue;
				}
			}

			if(Notlowest == 0)
			{
				lowestb = i;
				break;
			}

			else	Notlowest = 0;
		}
	}



	Notlowest = 0;

	if(GetType(c) == C4V_Array)
	{
		for(i in c)
		{
			for(y in c)
			{
				if(y < i)
				{
					Notlowest = 1;
					continue;
				}
			}

			if(Notlowest == 0)
			{
				lowestc = i;
				break;
			}
		}
	}



	Notlowest = 0;

	if(GetType(d) == C4V_Array)
	{
		for(i in d)
		{
			for(y in d)
			{
				if(y < i)
				{
					Notlowest = 1;
					continue;
				}
			}

			if(Notlowest == 0)
			{
				lowestd = i;
				break;
			}

			else	Notlowest = 0;
		}
	}



	var Highest = HighestElemInAr([lowesta] , [lowestb] , [lowestc] , [lowestd]);

	if(GetType(b) != C4V_Array)	lowestb = Highest +1;
	if(GetType(c) != C4V_Array)	lowestc = Highest +1;
	if(GetType(d) != C4V_Array)	lowestd = Highest +1;


	if(lowesta < lowestb && lowesta < lowestc && lowesta < lowestd)		return(lowesta);
	if(lowestb < lowesta && lowestb < lowestc && lowestb < lowestd)		return(lowestb);
	if(lowestc < lowesta && lowestc < lowestb && lowestc < lowestd)		return(lowestc);
	if(lowestd < lowesta && lowestd < lowestb && lowestd < lowestc)		return(lowestd);



}





//Gibt das näheste Objekt eines bereits bestehenden Arrays zurück, welcher nur aus Objekten besteht
global func NearObj(array obj)
{
	var i = 0;

	for(i in obj)
	{
		if((!GetType() == C4V_C4Object))
		{
			return false;
		}
	}


	var pObj;

	var aDist = CreateArray();

	for(i in obj)
	{
		aDist[GetLength(aDist)] = ObjectDistance(i);
	}

	var Index = GetIndexOf(LowestElemInAr(aDist) , aDist);



	pObj = obj[Index];

	return(pObj);
}

//Löscht gleiche Elemente aus einem(oder zwei) Array(s)
global func RemoveSameElem(array a , array b)
{
	var bSame = 0;

	var aBool = GetType(a) == C4V_Array;
	var bBool = GetType(b) == C4V_Array;

	if(aBool && !bBool)
	{
		bSame = a[0];

		for(var i in a)
		{
			for(var x in a)
			{
				if(x == i && bSame != i)
				{
					bSame = i;
					PopElement(a , GetIndexOf(x , a));
				}
			}
		}

		return(a);
	}

	if(bBool && !aBool)
	{
		bSame = b[0];

		for(var i in b)
		{
			for(var x in b)
			{
				if(x == i && bSame != i)
				{
					bSame = i;
					PopElement(b , GetIndexOf(x , b));
				}
			}
		}

		return(b);
	}



	if(bBool && aBool)
	{
		for(var i in a)
		{
			for(var x in b)
			{
				if(x == i)
				{
					PopElement(a , GetIndexOf(x , a));
					PopElement(b , GetIndexOf(x , b));
				}
			}
		}

		var failsave = a;
		a = JoinArrays(failsave , b);

		return(a);
	}

}


//Set die X-Position eines Objektes
global func SetX(int iX , object pObj)
{
	return SetPosition(iX , GetY(pObj) , pObj);
}


//Set die Y-Position eines Objektes
global func SetY(int iY , object pObj)
{
	return SetPosition(GetX(pObj) , iY , pObj);
}


//Schafft am Anfang des angegebenen Arrays soviele frei Plätze wie angegeben
global func ShiftArray(array aArray , int iTime)
{
	var iTimes = iTime;
	if(iTimes == 0)	iTimes = 1;

	var a = aArray;

	for(var i = GetLength(a) -1; i > 0; i--)
	{
		a[i +iTimes] = a[i];
		a[i] = 0;

		if(i == 1)
		{
			a[iTimes] = a[0];
			a[0] = 0;
		}
	}

	return a;
}


//Lässt ein Objekt auf ein anderes Objekt fliegen(mein ganzer Stolz)
global func ShootAt(object pTarget , int iPrec)
{
	var iPrecision = iPrec;
	if(iPrecision)	iPrec = 10;

	var iXDistance = GetX(pTarget) - GetX();
	var iYDistance = GetY(pTarget) - GetY();
	var iTime = ObjectDistance(this() , pTarget) / 7;
	var iGravity = iPrecision * GetGravity() / 500;
	var iXDir = iXDistance * iPrecision / iTime;
	var iYDir = ((iYDistance * iPrecision / iTime * 2) - (iGravity * iTime));

	if(iYDir % 2)	iYDir ++;
	if(Contained())	Exit();


	SetXDir(iXDir , this() , iPrecision);
	SetYDir(iYDir / 2 , this() , iPrecision);

	return true;
}







//Löst eine Verhältnisgleichung(das unbekannte Glied muss immer "i4" sein)
global func SolveProportion(int i1 , int i2 , int i3 , int i4)
{
	var iResult = (i2 * i3) / i1;
	i4 = iResult;

	if(!IsRef(i4))	return iResult;
	else		return true;
}


//Für faule Entwickler
global func T(iany)
{
	if(iany == 0 && !IsRef(iany))	Log("Test%d!" , Random(1000));
	else				Log("%v (%d)" , iany , Random(1000));

	return true;
}



//Gibt den ggt(größter gemeinsamer Teiler) des angegebenen Arrays zurück, welcher nur aus Integern besteht
global func ggt(array aArray)
{
	var a = aArray;
	var iLowest = LowestElemInAr(a);
	var i;
	var iElement;
	var fElement;
	var fBool = true;
	var fDivisible = [];

	for(i = iLowest; i > 0; i --)
	{
		for(iElement in a)
		{
			if(GetType(iElement) != C4V_Int)	PopElement(a , GetIndexOf(iElement , a));

			else
			{
				if(!IsDivisible(iElement , i))
				{
					fDivisible[GetLength(fDivisible)] = false;
					continue;
				}

				else	fDivisible[GetLength(fDivisible)] = true;
			}
		}

		for(fElement in fDivisible)
		{
			if(!fElement)
			{
				fBool = false;
				continue;
			}
		}

		if(fBool)	return i;

		fBool = true;
		fDivisible = [];
	}
}




//Gibt das kgv(kleinstes gemeinsames Vielfaches) des angegebenen Arrays zurück, welcher nur aus Integern besteht
global func kgv(array aArray)
{
	if(GetLength(aArray) == 1)	return aArray[0];

	var iResult = kgv(aArray[0] , aArray[1]);

	for(var i = 2; i < GetLength(aArray); ++i)
	{
		iResult = kgv(iResult , aArray[i]);
	}

	return iResult;
}





/*
Gibt die Position der TopFace eines Objektes als Array zurück(Koordinaten sind global und
Die Elemente sind:
1. X-Position,
2. Y-Position,
3. X-Position der breite,
4. Y-Position der Breite,
5. X-Position der Höhe,
6. Y-Position der Höhe)
*/

protected func GetTopFacePosition(object pObj)
{
	var iX = GetX(pObj);
	var iY = GetY(pObj);
	var iTopX = GetDefCoreVal("TopFace" , "DefCore" , GetID(pObj) , 0);
	var iTopY = GetDefCoreVal("TopFace" , "DefCore" , GetID(pObj) , 1);
	var iWidth = GetDefCoreVal("TopFace" , "DefCore" , GetID(pObj) , 2);
	var iHeight = GetDefCoreVal("TopFace" , "DefCore" , GetID(pObj) , 3);
	var a = [iX + iTopX , iY + iTopY , iX + iWidth , iY + iWidth , iX + iHeight , iY + iHeight];
}





//Überladung für ResetVertices
global func RemoveVertex(int iIndex , object pObj , bool fSave)
{
	if(fSave)
	{
		if(GetType(Local(14253 , pObj)) != C4V_Array)	Local(14253 , pObj) = [];
		if(GetType(Local(14254 , pObj)) != C4V_Array)	Local(14254 , pObj) = [];

		Local(14253 , pObj)[Local(14253 , pObj)] = GetVertex(iIndex , 0 , pObj);
		Local(14254 , pObj)[Local(14254 , pObj)] = GetVertex(iIndex , 1 , pObj);
	}

	return _inherited(iIndex , pObj);
}


//Setzt die Vertex zurück
global func ResetVertices(object pObj)
{
	if(GetType(Local(14253 , pObj)) != C4V_Array || GetType(Local(14254 , pObj)) != C4V_Array)
		return false;

	for(var i = 0; i < GetLength(Local(14253 , pObj)); i ++)	AddVertex(Local(14253 , pObj)[i] , Local(14254 , pObj)[i] , pObj);

	Local(14253 , pObj) = [];
	Local(14254 , pObj) = [];

	return true;
}



//Gibt die relative X-Position eines Vertex eines Objektes zurück
global func GetVertexX(int iIndex , object pObj)
{
	return GetVertex(iIndex , 0 , pObj);	
}

//Gibt die relative Y-Position eines Vertex eines Objektes zurück
global func GetVertexY(int iIndex , object pObj)
{
	return GetVertex(iIndex , 1 , pObj);	
}



//Erzeugt eine imaginäre Locale Variable mit Name
global func CreateLocalN(string szLocalName , xValue , object pTargetObject)
{
	if(GetType(Local(10001)) != C4V_Array)	Local(10001) = [];

	Local(10001)[Local(10001)] = szLocalName;
	Local(10002)[Local(10002)] = xValue;

	return true;
}



//Reload für CreateLocalN (benötigt Pitris Funktion "PartOf")
/* global func LocalN(string szLocalName , object pLocalObject)
{
	if(!_inherited(...))
	{
		if(PartOf(szLocalName , Local(10001 , pLocalObject)) && GetLength(Local(10002 , pLocalObject)))
			return (Local(10002 , pLocalObject)[GetIndexOf(szLocalName , Local(10001 , pLocalObject))]);
	}

	else	return _inherited(...);
} */
// for now i commented this out
//TODO: fix this



//Erweitert! Man kann die Funktion jetzt auch mitten im Spiel benutzen und ganze Scripte ausführen lassen...
global func AddMsgBoardCmd(string szBoardCmd , string szScript , string szFunction , object pFunctionObject)
{
}



//Das Gegenteil von AbsX ("C" steht für "Changed")
global func CAbsX(int iX)
{
	return (iX - AbsX(GetX()));
}

//Das Gegenteil von AbsY ("C" steht für "Changed")
global func CAbsY(int iY)
{
	return (iY - AbsX(GetY()));
}








/*------------------------------------------------------ GBack*-Funktionen ------------------------------------------------------*/
//Alle Selbsterklärend


/*-------------------------- GBack*Rect-Funktionen --------------------------*/


global func GBackSolidRect(int x , int y, int iWidth , int iHeight)
{
	var iWid = Abs(iWidth);
	var iHei = Abs(iHeight);

	for(var i = y; i <= iHei; i ++)
	{
		for(var z = x; z <= iWid; z ++)
		{
			if(!GBackSolid(i , z))	return false;
		}
	}

	return true;
}

global func GBackSemiSolidRect(int x , int y, int iWidth , int iHeight)
{
	var iWid = Abs(iWidth);
	var iHei = Abs(iHeight);

	for(var i = y; i <= iHei; i ++)
	{
		for(var z = x; z <= iWid; z ++)
		{
			if(!GBackSemiSolid(i , z))	return false;
		}
	}

	return true;
}

global func GBackLiquidRect(int x , int y, int iWidth , int iHeight)
{
	var iWid = Abs(iWidth);
	var iHei = Abs(iHeight);

	for(var i = y; i <= iHei; i ++)
	{
		for(var z = x; z <= iWid; z ++)
		{
			if(!GBackLiquid(i , z))	return false;
		}
	}

	return true;
}

global func GBackSkyRect(int x , int y, int iWidth , int iHeight)
{
	var iWid = Abs(iWidth);
	var iHei = Abs(iHeight);

	for(var i = y; i <= iHei; i ++)
	{
		for(var z = x; z <= iWid; z ++)
		{
			if(!GBackSky(i , z))	return false;
		}
	}

	return true;
}



global func GBackSolidAtRect(int x , int y, int iWidth , int iHeight)
{
	var i;
	var z;

	var iWid = Abs(iWidth);
	var iHei = Abs(iHeight);

	for(i = x; i <= iWid; i ++)
	{
		if(!GBackSolid(i , y))	return false;
	}

	for(z = y; z <= iHei; z ++)
	{
		if(!GBackSolid(x , z))	return false;
	}

	for(i = x; i <= iWid; i ++)
	{
		if(!GBackSolid(i , y + iHei))	return false;
	}

	for(z = y; z <= iHei; z ++)
	{
		if(!GBackSolid(x + iWid , z))	return false;
	}

	return true;
}

global func GBackSemiSolidAtRect(int x , int y, int iWidth , int iHeight)
{
	var i;
	var z;

	var iWid = Abs(iWidth);
	var iHei = Abs(iHeight);

	for(i = x; i <= iWid; i ++)
	{
		if(!GBackSemiSolid(i , y))	return false;
	}

	for(z = y; z <= iHei; z ++)
	{
		if(!GBackSemiSolid(x , z))	return false;
	}

	for(i = x; i <= iWid; i ++)
	{
		if(!GBackSemiSolid(i , y + iHei))	return false;
	}

	for(z = y; z <= iHei; z ++)
	{
		if(!GBackSemiSolid(x + iWid , z))	return false;
	}

	return true;
}

global func GBackLiquidAtRect(int x , int y, int iWidth , int iHeight)
{
	var i;
	var z;

	var iWid = Abs(iWidth);
	var iHei = Abs(iHeight);

	for(i = x; i <= iWid; i ++)
	{
		if(!GBackLiquid(i , y))	return false;
	}

	for(z = y; z <= iHei; z ++)
	{
		if(!GBackLiquid(x , z))	return false;
	}

	for(i = x; i <= iWid; i ++)
	{
		if(!GBackLiquid(i , y + iHei))	return false;
	}

	for(z = y; z <= iHei; z ++)
	{
		if(!GBackLiquid(x + iWid , z))	return false;
	}

	return true;
}

global func GBackSkyAtRect(int x , int y, int iWidth , int iHeight)
{
	var i;
	var z;

	var iWid = Abs(iWidth);
	var iHei = Abs(iHeight);

	for(i = x; i <= iWid; i ++)
	{
		if(!GBackSky(i , y))	return false;
	}

	for(z = y; z <= iHei; z ++)
	{
		if(!GBackSky(x , z))	return false;
	}

	for(i = x; i <= iWid; i ++)
	{
		if(!GBackSky(i , y + iHei))	return false;
	}

	for(z = y; z <= iHei; z ++)
	{
		if(!GBackSky(x + iWid , z))	return false;
	}

	return true;
}






/*-------------------------- GBack*Circle-Funktionen --------------------------*/


global func GBackSolidCircle(x , y , iRadius)
{
	var iRad = Abs(iRadius);

	for(var z = iRad; z > 0; z --)
	{
		for(var i = 0; i < iRad; i ++)
		{
			if(!GBackSolid(Cos(i , z) + x , Sin(i , z) + y))	return false;
		}
	}

	return true;
}

global func GBackSemiSolidCircle(x , y , iRadius)
{
	var iRad = Abs(iRadius);

	for(var z = iRad; z > 0; z --)
	{
		for(var i = 0; i < iRad; i ++)
		{
			if(!GBackSemiSolid(Cos(i , z) + x , Sin(i , z) + y))	return false;
		}
	}

	return true;
}

global func GBackLiquidCircle(x , y , iRadius)
{
	var iRad = Abs(iRadius);

	for(var z = iRad; z > 0; z --)
	{
		for(var i = 0; i < iRad; i ++)
		{
			if(!GBackLiquid(Cos(i , z) + x , Sin(i , z) + y)) return false;
		}
	}

	return true;
}

global func GBackSkyCircle(x , y , iRadius)
{
	var iRad = Abs(iRadius);

	for(var z = iRad; z > 0; z --)
	{
		for(var i = 0; i < iRad; i ++)
		{
			if(!GBackSky(Cos(i , z) + x , Sin(i , z) + y))	return false;
		}
	}

	return true;
}



global func GBackSolidAtCircle(x , y , iRadius)
{
	var iRad = Abs(iRadius);

	for(var z = iRad; z > 0; z --)
	{
	for(var i = 0; i < iRad; i ++)
	{
		if(!GBackSolid(Cos(i , z) + x , Sin(i , z) + y))	return false;
	}
	}

	return true;
}

global func GBackSemiSolidAtCircle(x , y , iRadius)
{
	var iRad = Abs(iRadius);

	for(var z = iRad; z > 0; z --)
	{
	for(var i = 0; i < iRad; i ++)
	{
		if(!GBackSemiSolid(Cos(i , z) + x , Sin(i , z) + y))	return false;
	}
	}
	return true;
}

global func GBackLiquidAtCircle(x , y , iRadius)
{
	var iRad = Abs(iRadius);
	
	for(var z = iRad; z > 0; z --)
	{
	for(var i = 0; i < iRad; i ++)
	{
		if(!GBackLiquid(Cos(i , z) + x , Sin(i , z) + y))	return false;
	}
	}

	return true;
}

global func GBackSkyAtCircle(x , y , iRadius)
{
	var iRad = Abs(iRadius);

	for(var z = iRad; z > 0; z --)
	{
	for(var i = 0; i < iRad; i ++)
	{
		if(!GBackSky(Cos(i , z) + x , Sin(i , z) + y))	return false;
	}
	}

	return true;
}








/*-------------------------- Dir*-Funktionen --------------------------*/



global func GetDir2(object pObj) { return GetDir(pObj) * 2 -1; }		//Gibt für die Richtung Links -1 und für die Richtung Rechts 1 zurück
global func GetCDir2(object pObj) { return (-1 * (GetDir(pObj) * 2 -1)); }	//Gibt für die Richtung Links 1 und für die Richtung Rechts -1 zurück
global func GetCDir(object pObj) { return GetDir(pObj) * -1 + 1; }		//Gibt für die Richtung Links 1 und für die Richtung Rechts 0 zurück