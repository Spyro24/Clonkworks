/*-- Cäsars Funktionen --*/

#strict 2

//Entfernt das angegebene Element aus dem angegebenen Array(der 2. Parameter ist der Index des Elements)
global func PopElement(array &a, int i)
{
	var r = a[i];
	while(++i < GetLength(a)) a[i-1] = a[i];

	SetLength(a, GetLength(a)-1);
	return r;
}

