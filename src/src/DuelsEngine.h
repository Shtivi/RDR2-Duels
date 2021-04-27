#pragma once

enum class DuelChallengeReaction : int
{
	Accept,
	Decline,
	Flee
};

#include "Duel.h"

class DuelsEngine
{
private:
	

public:
	DuelsEngine();
	void update();

	static DuelChallengeReaction generatePedDuelReaction(Ped candidate);

private:

};