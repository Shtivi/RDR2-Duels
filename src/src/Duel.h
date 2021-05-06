#pragma once

#include "Main.h"

enum class DuelStage : int
{
	Challenged,
	Accpeted,
	Positioning,
	Drawing,
	PostDuel,
	Combat,
	PlayerWon,
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
	Stopwatch drawTimer;
	int opponentDrawTime;
	GameCamera* duelCamera;
	int duelShockingEvent;
	bool isDuelWellPositioned;

public:
	Duel(Ped challengedPed, Position pos1, Position pos2);
	void update();
	DuelStage getStage();
	bool isRunning();
	void cleanup();

private:
	void setStage(DuelStage stage);
	void onPedChallenged();
	void handleDuelReaction(DuelChallengeReaction reaction);
	void onDuelAccepted();
	void generateDuelPosition();
	void onPositioningInitiated();
	bool isPositioningCompleted();
	void enterDrawMode();
	void onDrawModeEntered();
	void onPlayerBailed();
	void onPlayerDirtyMove();
	void onPlayerDrew();
	void onOpponentDeclined();
	void onDuelWon();
};