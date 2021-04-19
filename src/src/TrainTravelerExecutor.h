#pragma once

class TrainTravelerExecutor : public AssassinationMissionExecutor
{
private:
	MapArea* area;
	Vehicle train;
	bool wasTargetSpooked;
	float initialDistanceToTrain;

public:
	TrainTravelerExecutor(MissionData* missionData, MissionStatus status, MapArea* area);
	virtual void update();

protected:
	virtual Vehicle spawnTrain();
	virtual bool wasTargetJustFound();
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
	virtual void cleanup();
	virtual bool waitForTrainBoarding();
	virtual Ped findTargetPed(int carriageBoarded);
	virtual void onAreaAbandoned();
	virtual bool willAreaBeAbandoned();
};