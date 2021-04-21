#include "Main.h"

const float DuelDistance = 12;

Prompt* challengePrompt;
Duel* duel;

DuelsEngine::DuelsEngine()
{
	challengePrompt = new Prompt("Duel", GAMEPLAY::GET_HASH_KEY("INPUT_INTERACT_OPTION1"), PromptMode::Standard);

	challengePrompt->hide();
}

void DuelsEngine::update()
{
	if (!duel)
	{
		Entity targetEntity = getPlayerTargetEntity();
		if (targetEntity && doesPedHaveSidearm(targetEntity))
		{
			challengePrompt->show();
			challengePrompt->setTargetEntity(targetEntity);

			if (challengePrompt->isActivatedByPlayer())
			{
				duel = new Duel(
					targetEntity,
					getClosestVehicleNode(playerPos()),
					getClosestVehicleNode(playerPos() + getForwardVector(player) * DuelDistance, true)
				);
			}
		}
	}
	else
	{
		challengePrompt->hide();
		duel->update();
		if (!duel->isRunning())
		{
			duel->cleanup();
			duel = NULL;
		}
	}
}

bool isPedDuelable(Ped ped)
{
	// TODO: Check if ped is a lawman
	return
		!ENTITY::IS_ENTITY_DEAD(ped) &&
		PED::IS_PED_HUMAN(ped) &&
		!ENTITY::IS_ENTITY_A_MISSION_ENTITY(ped) &&
		PED::IS_PED_ON_FOOT(ped) &&
		doesPedHaveSidearm(ped) &&
		!PED::IS_PED_IN_COMBAT(ped, player) &&
		!AI::IS_PED_RUNNING(ped) &&
		!AI::IS_PED_SPRINTING(ped) && 
		!PED::IS_PED_IN_MELEE_COMBAT(ped) &&
		!PED::_0x3AA24CCC0D451379(ped)/* IS_PED_HOGTIED */;
}