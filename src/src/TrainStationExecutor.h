#pragma once

class TrainStationExecutor : public AssassinationMissionExecutor
{
public:
	TrainStationExecutor(MissionData* missionData, MissionStatus status);
	virtual void update();

protected:
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
	bool playRandomSeatedScenario(Ped ped);
};