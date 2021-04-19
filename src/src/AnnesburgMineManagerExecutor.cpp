#include "Main.h"

AnnesburgMineManagerExecutor::AnnesburgMineManagerExecutor(MissionData* missionData, MissionStatus status)
	: AssassinationMissionExecutor(missionData, status)
{
}

void AnnesburgMineManagerExecutor::prepareSet()
{

	int OFFICE_CHAIR_MODAL_HASH = 70472470;
	Vector3 location = getMissionData()->startPosition;
	Object chair = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(location.x, location.y, location.z, 3, OFFICE_CHAIR_MODAL_HASH, 0, 0, 0);
	ScenarioPoint scenarioPoint(chair, "PROP_HUMAN_SEAT_CHAIR_SKETCHING", toVector3(0, 0, 0.5), 180);
	scenarioPoint.create();
	AI::_0xCCDAE6324B6A821C(getTargetPed(), scenarioPoint.getId(), 0, -1, 0, 1, 0, 0, -1082130432, 0);
}

Ped AnnesburgMineManagerExecutor::spawnTarget()
{
	Ped targetPed = createPed("a_m_m_gamhighsociety_01", getMissionData()->startPosition);
	DECORATOR::DECOR_SET_INT(targetPed, "loot_money", rndInt(1500, 2500));
	return targetPed;
}

const char* AnnesburgMineManagerExecutor::generateTrackDownMessage()
{
	return "Find and assassinate the mine's manager.";
}