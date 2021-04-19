#pragma once

class TrainWorkerExecutor : public AssassinationMissionExecutor
{
private:
	MapArea* area;
	Vehicle hardcart;
	vector<Ped> guards;
	vector<Ped> workers;
	int groupId;
	bool wasCombatTriggered;

public:
	TrainWorkerExecutor(MissionData* missionData, MissionStatus status, MapArea* area);
	virtual void update();

protected:
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
	virtual bool shouldCombatStart();
	virtual void beginCombat();
	virtual Ped addWorker(bool asMember = true);
	virtual Ped addGuard();
};