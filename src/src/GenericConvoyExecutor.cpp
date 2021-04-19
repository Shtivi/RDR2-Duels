#include "Main.h"

GenericConvoyExecutor::GenericConvoyExecutor(MissionData* missionData, MissionStatus status, ConvoyExecutorArgs params)
	: AssassinationMissionExecutor(missionData, status)
{

	spawnLocation = getClosestRoad(missionData->startPosition, 100);
	wagon = NULL;
	enemiesGroup = NULL;
	this->params = params;
}

void GenericConvoyExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (enemiesGroup)
	{
		enemiesGroup->update();
	}
}

void GenericConvoyExecutor::prepareSet()
{
	addRider();
	addRider();

	if (getMissionData()->difficultyLevel == MissionDifficulty::Hard)
	{
		addRider();
	}
	else if (getMissionData()->difficultyLevel > MissionDifficulty::Hard)
	{
		addRider();
		addRider();
	}

	//for (int i = 1; i <= params.additionalVehicles; i++)
	//{
	//	spawnVehicle(entityPos(wagon) + getForwardVector(wagon) * 15 * i);
	//}
}

Ped GenericConvoyExecutor::spawnTarget()
{
	wagon = spawnVehicle(spawnLocation, false, false);
	log("Wagon spawned");

	Ped targetPed = createPed(params.enemiesModel, wagon, -1);
	DECORATOR::DECOR_SET_INT(targetPed, "honor_override", 0);

	Ped shotgun = createPed(params.enemiesModel, wagon, -1);
	DECORATOR::DECOR_SET_INT(targetPed, "honor_override", 0);

	enemiesGroup = new RidersGroup(targetPed, 15, 7, true);
	enemiesGroup->addRider(shotgun);

	return targetPed;
}

const char* GenericConvoyExecutor::generateTrackDownMessage()
{
	return params.trackdownMessage;
	//return getMissionData()->getTemplate()->description;
}

Ped GenericConvoyExecutor::addRider()
{
	Vector3 pos = entityPos(getTargetPed()) + getForwardVector(getTargetPed()) * rndInt(-25, -8);
	Ped horse = addMissionEntity(createAmbientHorse(pos));
	Ped rider = addMissionEntity(createPedOnHorse((char*)params.enemiesModel, horse, -1));
	DECORATOR::DECOR_SET_INT(rider, "honor_override", 0);
	enemiesGroup->addRider(rider);
	return rider;
}

Vehicle GenericConvoyExecutor::spawnVehicle(Vector3 location, bool withDriver, bool withShotgun)
{
	Vehicle spawnedWagon = addMissionEntity(createVehicleOnClosestRoad(params.wagonModel, location));
	setVehicleCargo(spawnedWagon, params.cargoHash);
	
	//ENTITY::SET_ENTITY_HEADING(spawnedWagon, ENTITY::GET_ENTITY_HEADING(wagon));

	if (withDriver)
	{
		Ped driver = addMissionEntity(createPed(params.enemiesModel, spawnedWagon, -1));
		enemiesGroup->addRider(driver);
		DECORATOR::DECOR_SET_INT(driver, "honor_override", 0);
	}

	if (withShotgun)
	{
		Ped shotgun = addMissionEntity(createPed(params.enemiesModel, spawnedWagon, 0));
		enemiesGroup->addRider(shotgun);
		DECORATOR::DECOR_SET_INT(shotgun, "honor_override", 0);
	}

	return spawnedWagon;
}