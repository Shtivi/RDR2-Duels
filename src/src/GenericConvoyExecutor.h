#pragma once

class ConvoyExecutorArgs
{
public:
	VehicleHash wagonModel;
	VehicleCargoHash cargoHash;
	const char* enemiesModel;
	const char* trackdownMessage;
	int additionalVehicles = 1;
};

class GenericConvoyExecutor : public AssassinationMissionExecutor
{
private:
	ConvoyExecutorArgs params;
	Vector3 spawnLocation;
	Vehicle wagon;
	RidersGroup* enemiesGroup;

public:
	GenericConvoyExecutor(MissionData* missionData, MissionStatus status, ConvoyExecutorArgs params);
	virtual void update();

protected:
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
	Ped addRider();
	Vehicle spawnVehicle(Vector3 location, bool withDriver = true, bool withShotgun = true);
};