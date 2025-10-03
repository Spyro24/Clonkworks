/* --- Böse Objektfunktionen --- */

#strict 2

//Beide von ST-DDT!!!

//Klont ein Objekt 
global func ObjectClone(object pObject, array anLocal, array anLocalN)
{
	if(!pObject)	pObject = this;
	if(!pObject)	return 0;

	var pClone = CreateObject(GetID(pObject), 0, 0, GetOwner(pObject));
	if(!pClone)	return false;

	ObjectDataTransfer(pObject, pClone, anLocal, anLocalN);
	SetClrModulation(GetClrModulation(pObject), pClone);

	if(pObject->Contained())	pClone -> Enter();

	//Alles was das Script nicht kann nachholen (Farbe, Registrierung, etc)
	pClone->~FinishCloning(pObject);
	return pClone;
}

//Kopiert alle gewünschten Daten von pObject nach pTarget
global func ObjectDataTransfer(object pObject, object pTarget, array anLocal, array anLocalN)
{
	if(!pObject)	return false;
	if(!pTarget)	return false;

	if(anLocal)
	{
		for(var dInt in anLocal)
		{
			Local(dInt, pTarget) = Local(dInt, pObject);
		}
	}

	if(anLocalN)
	{
		for(var sName in anLocalN)
		{
			Local(sName, pTarget) = Local(sName, pObject);
		}
	}

	return true;
}
