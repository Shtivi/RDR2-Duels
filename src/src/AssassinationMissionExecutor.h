#pragma once

class AssassinationMissionExecutor
{
private:
	MissionData* missionData;
	MissionObjective objective;
	MissionStatus status;
	Challenges* challenges;

protected:
	Blip areaBlip;
	Ped target;
	Blip targetBlip;
	Stopwatch stopwatch;

protected:
	vector<Entity> missionEntities;

public:
	AssassinationMissionExecutor(MissionData* missionData, MissionStatus status = MissionStatus::Unavailable);

	MissionData* getMissionData();
	MissionObjective getObjective();
	MissionStatus getStatus();
	void setStatus(MissionStatus status);
	int getTotalReward();

	virtual void update();
	void resume();
	void block();

protected:

	float getPlayerDistanceToArea();
	virtual Ped getTargetPed();

	virtual void initialize();
	virtual void nextObjective();
	virtual void onAccepted();
	virtual void onAreaReached();
	virtual void onAreaAbandoned();
	virtual bool wasTargetJustFound();
	virtual void onTargetFound();
	virtual void onTargetKilled();
	virtual void onEscapedArea();
	virtual void onCompleted();
	virtual void updateOptionalObjectives();
	virtual void prepareSet() = 0;
	virtual Ped spawnTarget() = 0;
	virtual const char* generateTrackDownMessage() = 0; 
	void fail(const char* reason, bool hardDeleteCleaup = false);
	
	virtual bool willAreaBeAbandoned();
	virtual Entity addMissionEntity(Entity entity);
	virtual void releaseEntitySafely(Entity* entity);
	virtual void cleanup(bool hardDelete = false);
	float calculateBonuses();
	void displayObjectives();

private:
	void decorateTarget();
};

#include "TransportedPrisonerExecutor.h"
#include "TrainStationExecutor.h"
#include "BusinessManConvoyExecutor.h"
#include "LawmanExecutor.h"
#include "SaloonExecutor.h"
#include "JailPrisonerExecutor.h"
#include "BanditsCampExecutor.h"
#include "RidersExecutor.h"
#include "HunterExecutor.h"
#include "AmbientPedExecutor.h"
#include "AnnesburgMineManagerExecutor.h"
#include "BountyHuntersRidersExecutor.h"
#include "GenericShackExecutor.h"
#include "GenericConvoyExecutor.h"
#include "TrainTravelerExecutor.h"
#include "TrainWorkerExecutor.h"
#include "HotelRoomExecutor.h"