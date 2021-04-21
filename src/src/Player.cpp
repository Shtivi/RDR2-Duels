#include "Main.h"

Entity getPlayerTargetEntity()
{
	Entity targetEntity;
	if (PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &targetEntity))
	{
		return targetEntity;
	}

	return NULL;
}