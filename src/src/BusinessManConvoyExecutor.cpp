#include "Main.h"

vector<const char*> BUSINESS_MAN_TARGET_MODELS = {
	"a_m_m_gamhighsociety_01",
	"a_m_m_grifancytravellers_01",
	"a_m_m_htlfancytravellers_01",
	"a_m_m_strfancytourist_01",
	"a_m_m_blwupperclass_01",
	"a_m_m_nbxupperclass_01",
	"a_m_m_rhdupperclass_01",
	"a_m_m_uppertrainpassengers_01"
};

vector<const char*> BODYGUARDS_MODELS = {
	"a_m_m_bivroughtravellers_01",
	"a_m_m_bynroughtravellers_01",
	"a_m_m_griroughtravellers_01",
	"a_m_m_htlroughtravellers_01"
};

BusinessManConvoyExecutor::BusinessManConvoyExecutor(MissionData* missionData, MissionStatus status)
	: AssassinationMissionExecutor(missionData, status)
{

	spawnLocation = getClosestRoad(missionData->startPosition, 100);
	targetStagecoach = NULL;
	enemiesGroup = NULL;
}

void BusinessManConvoyExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (enemiesGroup)
	{
		enemiesGroup->update();
	}
}

void BusinessManConvoyExecutor::prepareSet()
{
	Ped shotgun = addMissionEntity(createPed("a_m_m_bivroughtravellers_01", targetStagecoach, 0));
	Ped driver = addMissionEntity(createPed("a_m_m_bivroughtravellers_01", targetStagecoach, -1));
	enemiesGroup = new RidersGroup(driver, 12, 6);
	enemiesGroup->addRider(shotgun);
	addRider();
	addRider();
	addRider();

	if (getMissionData()->difficultyLevel > MissionDifficulty::Normal) 
	{
		addRider();
	}

	enemiesGroup->addPassanger(getTargetPed());

	Ped wifePed = createPed("a_f_m_bivfancytravellers_01", targetStagecoach, 3);
	DECORATOR::DECOR_SET_INT(wifePed, "loot_money", rndInt(1500, 2500));
	enemiesGroup->addPassanger(wifePed);
}

Ped BusinessManConvoyExecutor::spawnTarget()
{
	VehicleHash targetVehicleHash = VehicleHash::StageCoach002X;
	if (getMissionData()->difficultyLevel == MissionDifficulty::Extreme)
	{
		targetVehicleHash = VehicleHash::StageCoach004X_Armoured;
	}

	targetStagecoach = addMissionEntity(createVehicleOnClosestRoad(targetVehicleHash, spawnLocation));
	const char* targetModel = BUSINESS_MAN_TARGET_MODELS.at(rndInt(0, BUSINESS_MAN_TARGET_MODELS.size()));
	Ped targetPed = createPed(targetModel, targetStagecoach, 2);
	DECORATOR::DECOR_SET_INT(targetPed, "loot_money", rndInt(1700, 2800));
	DECORATOR::DECOR_SET_INT(targetPed, "honor_override", 0);
	return targetPed;
}

void BusinessManConvoyExecutor::cleanup()
{
	AssassinationMissionExecutor::cleanup();

	for (vector<Entity>::iterator itr = missionEntities.begin(); itr != missionEntities.end(); itr++)
	{
		Entity curr = *itr;
		if (!ENTITY::DOES_ENTITY_EXIST(curr) || ENTITY::IS_ENTITY_DEAD(curr) || distance(playerPos(), curr) > 200)
		{
			ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&curr);
		}
	}
}

const char* BusinessManConvoyExecutor::generateTrackDownMessage()
{
	return "Track down the business man's convoy and assassinate him.";
}

void BusinessManConvoyExecutor::addRider()
{
	Vector3 pos = entityPos(targetStagecoach) + getForwardVector(targetStagecoach) * rndInt(-20, -8);
	Ped horse = addMissionEntity(createAmbientHorse(pos));
	const char* riderModel = BODYGUARDS_MODELS.at(rndInt(0, BODYGUARDS_MODELS.size()));
	Ped rider = addMissionEntity(createPedOnHorse((char*)riderModel, horse, -1));
	DECORATOR::DECOR_SET_INT(rider, "honor_override", 0);
	enemiesGroup->addRider(rider);
}