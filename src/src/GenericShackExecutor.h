#pragma once

class GenericShackExecutor : public AssassinationMissionExecutor
{
private:
	vector<Ped> enemies;
	Place* place;
	MapArea* area;
	bool hasCombatTriggered;

public:
	GenericShackExecutor(MissionData* missionData, MissionStatus status, Place* place, MapArea* area);
	virtual void update();

protected:
	virtual bool wasTargetJustFound();
	virtual void onTargetFound();
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual Ped addEnemy(Vector3 coords);
	virtual const char* generateTrackDownMessage();
	virtual void showEnemyBlips();
};