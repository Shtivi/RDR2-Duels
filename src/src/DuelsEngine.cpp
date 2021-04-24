#include "Main.h"

const float DuelDistance = 12;

Prompt* challengePrompt;
Duel* duel;

bool isPedDuelable(Ped ped)
{
	bool allow =
		!ENTITY::IS_ENTITY_DEAD(ped) &&
		PED::IS_PED_HUMAN(ped) &&
		!ENTITY::IS_ENTITY_A_MISSION_ENTITY(ped) &&
		PED::IS_PED_ON_FOOT(ped) &&
		!PED::IS_PED_IN_COMBAT(ped, player) &&
		doesPedHaveSidearm(ped) &&
		!AI::IS_PED_RUNNING(ped) &&
		!AI::IS_PED_SPRINTING(ped) &&
		!PED::IS_PED_FLEEING(ped) &&
		!AI::IS_PED_CUFFED(ped) &&
		!DECORATOR::DECOR_EXIST_ON(ped, "SH_DUELS_dueled") && 
		!PED::IS_PED_IN_MELEE_COMBAT(ped) &&
		!PED::_0x3AA24CCC0D451379(ped)/* IS_PED_HOGTIED */;

	if (isPedLawman(ped) && !ScriptSettings::getBool("DuelLawmen"))
	{
		return false;
	}

	return allow;
}

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
		if (targetEntity && isPedDuelable(targetEntity))
		{
			challengePrompt->show();
			challengePrompt->setTargetEntity(targetEntity);

			if (challengePrompt->isActivatedByPlayer())
			{
				log("Duel initiated");
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
			log("finalizing duel");
			duel->cleanup();
			duel = NULL;
		}
	}
}