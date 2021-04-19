#pragma once

class BountyHuntersRidersExecutor : public AssassinationMissionExecutor
{
private:
	Vector3 spawnLocation;
	RidersGroup* riders;
	MapArea* area;

public:
	BountyHuntersRidersExecutor(MissionData* missionData, MissionStatus status, MapArea* area);
	virtual void update();

protected:
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();

private:
	void addBountyHunter();
};