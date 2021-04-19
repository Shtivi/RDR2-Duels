#include "Main.h"

AmbientPedExecutor::AmbientPedExecutor(MissionData* missionData, MissionStatus status, MapArea* area, AmbientPedExecutorArgs params)
	: AssassinationMissionExecutor(missionData, status)
{
	this->area = area;
	this->params = params;
}


void AmbientPedExecutor::update()
{
	AssassinationMissionExecutor::update();
}

bool AmbientPedExecutor::wasTargetJustFound()
{
	return AssassinationMissionExecutor::wasTargetJustFound();
}

void AmbientPedExecutor::prepareSet()
{
	if (PED::IS_PED_IN_GROUP(getTargetPed()) && params.companions)
	{
		for (int i = 0; i < params.companions; i++) 
		{
			addCompanion();
		}
	}
}

Ped AmbientPedExecutor::spawnTarget()
{
	MissionData* missionData = getMissionData();
	Ped targetPed = getClosestPed(missionData->startPosition, min(params.lookupRadius, missionData->areaRadius));
	if (!targetPed ||
		ENTITY::IS_ENTITY_DEAD(targetPed) ||
		!PED::IS_PED_HUMAN(targetPed) ||
		ENTITY::IS_ENTITY_A_MISSION_ENTITY(targetPed) ||
		(params.pedModel && ENTITY::GET_ENTITY_MODEL(targetPed) != GAMEPLAY::GET_HASH_KEY((char*)params.pedModel)))
	{
		log("could not find an ambient ped around the start location, spawning a random typical ped");

		int group = PED::CREATE_GROUP(0);
		const char* model = params.pedModel ? params.pedModel : area->getRandomLocalGangPedModel();
		targetPed = createPed(model, *getSafeCoordForPed(missionData->startPosition));
		PED::SET_PED_AS_GROUP_LEADER(targetPed, group, 0);

		WAIT(500);
		Vector3 wanderLoc = getMissionData()->startPosition;
		AI::TASK_WANDER_IN_AREA(targetPed, wanderLoc.x, wanderLoc.y, wanderLoc.z, getMissionData()->areaRadius, 10, 10, 1);
	}
	else 
	{
		log("AmbientPedExecutor: proper ambient target have been found.");
	}

	return targetPed;
}

const char* AmbientPedExecutor::generateTrackDownMessage()
{
	return "Find and assassinate the target.";
}

void AmbientPedExecutor::addCompanion()
{
	Vector3 targetLocation = entityPos(getTargetPed());
	Vector3 spawnLocation = around(targetLocation, 5);
	Ped ped = addMissionEntity(createPed(params.pedModel ? params.pedModel : area->getRandomLocalGangPedModel(), spawnLocation));
	DECORATOR::DECOR_SET_INT(ped, "honor_override", 0);
	int group = PED::GET_PED_GROUP_INDEX(getTargetPed());
	PED::SET_PED_AS_GROUP_MEMBER(ped, group);
}