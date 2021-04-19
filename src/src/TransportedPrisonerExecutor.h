#pragma once

class TransportedPrisonerExecutor : public AssassinationMissionExecutor
{
private:
	Vector3 spawnLocation;
	Vehicle prisonWagon;
	RidersGroup* lawmenGroup;
	MapArea* area;

public:
	TransportedPrisonerExecutor(MissionData* missionData, MissionStatus status, MapArea* area);
	virtual void update();

protected:
	virtual void onTargetFound();
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
};