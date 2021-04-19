#pragma once

class JailPrisonerExecutor : public AssassinationMissionExecutor
{
public:
	JailPrisonerExecutor(MissionData* missionData, MissionStatus status);
	virtual void update();

protected:
	virtual bool wasTargetJustFound();
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
};