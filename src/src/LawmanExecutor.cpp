#include "Main.h"

LawmanExecutor::LawmanExecutor(MissionData* missionData, MissionStatus status)
	: AssassinationMissionExecutor(missionData, status)
{
	enemiesGroup = NULL;
}


void LawmanExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (enemiesGroup)
	{
		enemiesGroup->update();
	}
}

bool LawmanExecutor::wasTargetJustFound()
{
	return
		distanceBetweenEntities(player, getTargetPed()) <= getMissionData()->identificationDistance &&
		AssassinationMissionExecutor::wasTargetJustFound();
}

void LawmanExecutor::prepareSet()
{
	int minLawmenCount = 0;
	switch (getMissionData()->difficultyLevel)
	{
	case MissionDifficulty::Normal:
		minLawmenCount = 0;
		break;
	case MissionDifficulty::Hard:
		minLawmenCount = 2;
		break;
	case MissionDifficulty::Extreme:
		minLawmenCount = 4;
		break;
	}

	int lawmenCount = rndInt(minLawmenCount, minLawmenCount + 2);
	while (lawmenCount > 0)
	{
		addCompanion();
		lawmenCount--;
	}
}

Ped LawmanExecutor::spawnTarget()
{
	//Vector3 spawnLocation = getClosestRoad(getMissionData()->startPosition, 20);
	Vector3* spawnLocation = getSafeCoordForPed(getMissionData()->startPosition);
	if (!(*spawnLocation))
	{
		log("LawmanExecutor: could not find a safe coord, using default");
		*spawnLocation = getMissionData()->startPosition;
	}

	Ped targetPed = createPed(getLawModel(), *spawnLocation);
	DECORATOR::DECOR_SET_INT(targetPed, "loot_money", rndInt(2800, 3500));
	enemiesGroup = new RidersGroup(targetPed, 2, 10);

	return targetPed;
}

const char* LawmanExecutor::generateTrackDownMessage()
{
	return "Find the lawman and eliminate him.";
}

void LawmanExecutor::addCompanion()
{
	Vector3 targetLocation = entityPos(getTargetPed());
	Vector3 spawnLocation = around(targetLocation, 5);
	Ped ped = createPed(getLawModel(), spawnLocation);
	DECORATOR::DECOR_SET_INT(ped, "honor_override", 0);
	enemiesGroup->addRider(ped);
}

const char* LawmanExecutor::getLawModel()
{
	switch (getMissionData()->area)
	{
	case MapAreas::Blackwater:
		return "s_m_m_ambientblwpolice_01";
	case MapAreas::SaintDenis:
		return "s_m_m_ambientsdpolice_01";
	default:
		return "s_m_m_ambientlawrural_01";
	}
}