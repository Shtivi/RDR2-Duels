#pragma once

class BusinessManConvoyExecutor : public AssassinationMissionExecutor
{
private:
	Vector3 spawnLocation;
	Vehicle targetStagecoach;
	RidersGroup* enemiesGroup;

public:
	BusinessManConvoyExecutor(MissionData* missionData, MissionStatus status);
	virtual void update();

protected:
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual void cleanup();
	virtual const char* generateTrackDownMessage();
	void addRider();
};