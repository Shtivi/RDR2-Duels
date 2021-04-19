#pragma once

#include "QuestOfferInteractionBuilder.h"

enum class QuestOfferOutcome : int
{
	None,
	Accepted,
	Declined
};

class QuestOfferExecutor
{
private:
	MapAreas area;
	QuestOfferOutcome outcome;
	bool _isRunning;
	Ped questGiver;
	float initialDistance;
	Prompt* positiveReply;
	Prompt* negativeReply;
	bool hadGreeted = false;

public:
	QuestOfferExecutor(MapAreas area, Ped questGiver);
	bool isRunning();
	QuestOfferOutcome getOutcome();
	void start();
	void update();
	void stop();
	MapAreas getArea();

protected:
	void decorateQuestGiver(Ped questGiver);
	void greetPlayer();
	void cleanup();
	void setOutcome(QuestOfferOutcome outcome);
};