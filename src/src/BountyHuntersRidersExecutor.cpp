#include "Main.h"

BountyHuntersRidersExecutor::BountyHuntersRidersExecutor(MissionData* missionData, MissionStatus status, MapArea* area)
	: AssassinationMissionExecutor(missionData, status)
{
	this->area = area;
	spawnLocation = getClosestRoad(missionData->startPosition, 100);
	if (!spawnLocation)
	{
		spawnLocation = missionData->startPosition;
	}
	riders = NULL;
}


void BountyHuntersRidersExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (riders)
	{
		riders->update();
	}
}

void BountyHuntersRidersExecutor::prepareSet()
{
	int max = 3;
	int min = 2;

	if (getMissionData()->difficultyLevel > MissionDifficulty::Normal)
	{
		min = 3;
		max = 5;
	}

	int riders = rndInt(min, max + 1);
	for (int i = 0; i < riders; i++)
	{
		addBountyHunter();
	}
}

Ped BountyHuntersRidersExecutor::spawnTarget()
{
	Ped horse = addMissionEntity(createAmbientHorse(spawnLocation));
	Ped targetPed = createPedOnHorse("u_m_m_unibountyhunter_01", horse, -1);
	DECORATOR::DECOR_SET_INT(targetPed, "loot_money", rndInt(1200, 1600));
	WAIT(1000);
	riders = new RidersGroup(targetPed, 13, 7, true);
	return targetPed;
}

const char* BountyHuntersRidersExecutor::generateTrackDownMessage()
{
	return "Track down and kill the bounty hunter. He is travelling somewhere around.";
}

void BountyHuntersRidersExecutor::addBountyHunter()
{
	Vector3 pos = entityPos(getTargetPed()) + getForwardVector(getTargetPed()) * rndInt(-8, 8);
	Ped horse = addMissionEntity(createAmbientHorse(pos));
	char* model = rndInt(0, 2) ? "u_m_m_unibountyhunter_01" : "u_m_m_unibountyhunter_02";
	Ped rider = addMissionEntity(createPedOnHorse(model, horse, -1));
	DECORATOR::DECOR_SET_INT(rider, "honor_override", 0);
	DECORATOR::DECOR_SET_INT(rider, "loot_money", rndInt(450, 700));
	riders->addRider(rider);
	WAIT(500);
}