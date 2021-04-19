#pragma once

class LawmanExecutor : public AssassinationMissionExecutor
{
private:
	RidersGroup* enemiesGroup;

public:
	LawmanExecutor(MissionData* missionData, MissionStatus status);
	virtual void update();

protected:
	virtual bool wasTargetJustFound();
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
	void addCompanion();
	const char* getLawModel();
};