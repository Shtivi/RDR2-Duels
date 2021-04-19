#include "Main.h"

vector<const char*> TARGET_MODELS = {
	"a_m_m_uppertrainpassengers_01",
	"a_m_m_bivfancytravellers_01",
	"a_m_m_bynfancytravellers_01",
	"a_m_m_bivroughtravellers_01",
	"a_m_m_bynroughtravellers_01",
	"a_m_m_griroughtravellers_01",
	"a_m_m_htlroughtravellers_01"
};

vector<int> CHAIRS_AND_BENCHES = {
	-975920629/*, 1328906077*/, 1057555344, -1443906703, -1025740342
};

vector<const char*> SEATING_SCENARIOS = {
	"PROP_HUMAN_SEAT_CHAIR_READ_NEWSPAPER",
	"PROP_HUMAN_SEAT_CHAIR_CIGAR",
	"PROP_HUMAN_SEAT_CHAIR_DRINKING",
	"PROP_HUMAN_SEAT_CHAIR_SMOKING"
};

TrainStationExecutor::TrainStationExecutor(MissionData* missionData, MissionStatus status)
	: AssassinationMissionExecutor(missionData, status)
{
}

void TrainStationExecutor::update()
{
	AssassinationMissionExecutor::update();
}

void TrainStationExecutor::prepareSet()
{
	WAIT(500);
	Ped targetPed = getTargetPed();
	Vector3 spawnPosition = entityPos(targetPed);

	if (!playRandomSeatedScenario(targetPed))
	{
		AI::_0x524B54361229154F(targetPed, GAMEPLAY::GET_HASH_KEY("WORLD_HUMAN_WAITING_IMPATIENT"), -1, true, true, 0, true);
	}
}

Ped TrainStationExecutor::spawnTarget()
{
	int targetModelsSize = TARGET_MODELS.size();
	const char* targetModel = TARGET_MODELS.at(rndInt(0, targetModelsSize));
	Vector3 spawnPosition = around(getMissionData()->startPosition, 15, true);
	spawnPosition.z = getMissionData()->startPosition.z;
	Ped targetPed = createPed(targetModel, spawnPosition);

	return targetPed;
}

const char* TrainStationExecutor::generateTrackDownMessage()
{
	return "Find and assassinate the target around the train station";
}

bool TrainStationExecutor::playRandomSeatedScenario(Ped ped)
{
	Vector3 pedPos = entityPos(ped);
	for (vector<int>::iterator itr = CHAIRS_AND_BENCHES.begin(); itr != CHAIRS_AND_BENCHES.end(); itr++)
	{
		Object chair = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pedPos.x, pedPos.y, pedPos.z, 10, (*itr), 0, 0, 0);
		if (!chair)
		{
			continue;
		}

		Vector3 chairPos = entityPos(chair);
		Ped closestPed = getClosestPed(chairPos, 4);
		if (!closestPed)
		{
			const char* scenarioType = SEATING_SCENARIOS.at(rndInt(0, SEATING_SCENARIOS.size()));
			int scenarioPoint = AI::_0x794AB1379A74064D(chair, GAMEPLAY::GET_HASH_KEY((char*)scenarioType), 0, 0, 0.5f, 120 + ENTITY::GET_ENTITY_HEADING(chair), 0, -1, 1); // TASK_CREATE_SCENARIO_POINT_ATTACHED_TO_ENTITY
			AI::_0x5AF19B6CC2115D34(scenarioPoint, 23, 1);
			PED::ADD_SCENARIO_BLOCKING_AREA(chairPos.x - 0.75, chairPos.y - 0.75, chairPos.z - 0.75, chairPos.x + 0.75, chairPos.y + 0.75, chairPos.z + 0.75, false, 15);
			AI::_0xCCDAE6324B6A821C(ped, scenarioPoint, 0, -1, 0, 1, 0, 0, -1082130432, 0);
			return true;
		}
	}

	return false;
}