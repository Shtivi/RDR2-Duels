#include "Main.h";

Blip createBlip(Vector3 pos, Hash blipType, Hash blipSprite)
{
	Blip blip;
	Object dummyProp = createProp("p_shotGlass01x", pos, true, false, false);

	blip = MAP::BLIP_ADD_FOR_ENTITY(blipType, dummyProp); // Add blip for dummy prop
	if (blipSprite != 0)
	{
		MAP::SET_BLIP_SPRITE(blip, blipSprite, false);
	}
	ENTITY::FREEZE_ENTITY_POSITION(dummyProp, true);

	return blip;
}

Blip createBlip(Entity entity, Hash blipType, Hash blipSprite)
{
	Blip blip = MAP::BLIP_ADD_FOR_ENTITY(blipType, entity); 

	if (blipSprite != 0)
	{
		MAP::SET_BLIP_SPRITE(blip, blipSprite, false);
	}

	return blip;
}

Blip createBlip(Vector3 source, float radius, Hash blipType, Hash blipSprite)
{
	Blip blip = MAP::BLIP_ADD_FOR_RADIUS(blipType, source.x, source.y, source.z, radius); // add blip for area

	if (blipSprite != 0)
	{
		MAP::SET_BLIP_SPRITE(blip, blipSprite, false);
	}

	return blip;
}

void setBlipLabel(Blip blip, const char* label)
{
	MAP::_SET_BLIP_NAME(blip, MISC::VAR_STRING(10, "LITERAL_STRING", label));
}

void deleteBlipSafe(Blip* blip)
{
	if (MAP::DOES_BLIP_EXIST(*blip))
	{
		MAP::REMOVE_BLIP(blip);
	}
}