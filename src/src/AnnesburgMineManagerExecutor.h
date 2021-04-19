#pragma once

class AnnesburgMineManagerExecutor : public AssassinationMissionExecutor
{
public:
	AnnesburgMineManagerExecutor(MissionData* missionData, MissionStatus status);

protected:
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
};