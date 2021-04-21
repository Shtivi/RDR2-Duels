#pragma once

#include "Main.h"

enum class DuelStage : int
{
	Challenged,
	Accpeted,
	Positioning,
	Drawing,
	Combat,
	Declined,
	PlayerBailed,
	PlayerDirtyMove
};

class Duel
{
private:
	DuelStage stage;
	Position pos1;
	Position pos2;
	Ped challengedPed;
	Blip positionBlip;
	Prompt* drawPrompt;

public:
	Duel(Ped challengedPed, Position pos1, Position pos2);
	void update();
	DuelStage getStage();
	bool isRunning();
	void cleanup();

private:
	void setStage(DuelStage stage);
	void onPedChallenged();
	void onDuelAccepted();
	void generateDuelPosition();
	void onPositioningInitiated();
	bool isPositioningCompleted();
	void enterDrawMode();
	void onDrawModeEntered();
	void onPlayerBailed();
	void onPlayerDirtyMove();

};