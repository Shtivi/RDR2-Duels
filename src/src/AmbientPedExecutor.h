#pragma once

class AmbientPedExecutorArgs 
{
public:
	const char* pedModel = NULL;
	int companions = 0;
	int lookupRadius;
};

class AmbientPedExecutor : public AssassinationMissionExecutor
{
private:
	MapArea* area;
	AmbientPedExecutorArgs params;

public:
	AmbientPedExecutor(MissionData* missionData, MissionStatus status, MapArea* area, AmbientPedExecutorArgs params);
	virtual void update();

protected:
	virtual bool wasTargetJustFound();
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
	void addCompanion();
};