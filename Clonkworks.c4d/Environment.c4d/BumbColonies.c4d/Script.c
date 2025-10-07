
#strict

protected func Initialize()
{
  if(!ObjectCount(TIME)){
	  Log("Bumb Colonies require a Day/Night cycle, colonies disabled");
  }
}

global func ColoniesEnabled(){
	return(ObjectCount(BBCL));
}
