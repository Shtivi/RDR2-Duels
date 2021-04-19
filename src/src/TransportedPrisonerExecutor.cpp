#include "Main.h"

TransportedPrisonerExecutor::TransportedPrisonerExecutor(MissionData* missionData, MissionStatus status, MapArea* area)
	: AssassinationMissionExecutor(missionData, status)
{
	spawnLocation = getClosestRoad(missionData->startPosition, 100);
	prisonWagon = NULL;
	lawmenGroup = NULL;
	this->area = area;
}

void TransportedPrisonerExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (lawmenGroup)
	{
		lawmenGroup->update();
	}
}

void TransportedPrisonerExecutor::onTargetFound()
{
	AssassinationMissionExecutor::onTargetFound();
}


void TransportedPrisonerExecutor::prepareSet()
{
	Ped shotgun = addMissionEntity(createPed("S_M_M_DispatchLawRural_01", prisonWagon, 0));
	Ped driver = addMissionEntity(createPed("S_M_M_DispatchLawRural_01", prisonWagon, -1));

	lawmenGroup = new RidersGroup(driver, 12, 6);
	lawmenGroup->addRider(shotgun);
	lawmenGroup->addPassanger(getTargetPed());

	int riders = rndInt(1, 3);
	riders += (int)getMissionData()->difficultyLevel - 1;

	while (riders > 0)
	{
		Vector3 pos = entityPos(prisonWagon) + getForwardVector(prisonWagon) * rndInt(-16, -8);
		Ped horse = addMissionEntity(createAmbientHorse(pos));
		Ped rider = addMissionEntity(createPedOnHorse((char*)"S_M_M_DispatchLawRural_01", horse, -1));
		lawmenGroup->addRider(rider);
		riders--;
	}

	WAIT(1000);
}

Ped TransportedPrisonerExecutor::spawnTarget()
{
	prisonWagon = addMissionEntity(createVehicleOnClosestRoad(VehicleHash::WagonPrison01X, spawnLocation));
	if (!prisonWagon)
	{
		log("TransportedPrisonerExecutor: failed to spawn prison wagon on the closest road to:");
		log(to_string(spawnLocation));
	}

	const char* targetModel = getMissionData()->difficultyLevel == MissionDifficulty::Normal ? "cs_chainprisoner_02" : area->getRandomLocalGangPedModel();

	Ped targetPed = createPed(targetModel, prisonWagon, 2);
	DECORATOR::DECOR_SET_INT(targetPed, "honor_override", 0);

	return targetPed;
}

 const char* TransportedPrisonerExecutor::generateTrackDownMessage()
{
	return "Locate the prison convoy and assassinate the ~COLOR_RED~Target.";
}