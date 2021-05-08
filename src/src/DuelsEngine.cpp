#include "Main.h"

const float DuelDistance = 12;

Prompt* challengePrompt;
Duel* duel;

bool isPedDuelable(Ped ped)
{
	if (ScriptSettings::getBool("AntagonizeFirst") && DECORATOR::DECOR_GET_INT(ped, "SH_DUELS_duelable") != 1)
	{
		return false;
	}

	bool allow =
		!ENTITY::IS_ENTITY_DEAD(ped) &&
		PED::IS_PED_HUMAN(ped) &&
		PED::IS_PED_ON_FOOT(ped) &&
		!PED::IS_PED_IN_COMBAT(ped, player) &&
		doesPedHaveSidearm(ped) &&
		!AI::IS_PED_RUNNING(ped) &&
		!AI::IS_PED_SPRINTING(ped) &&
		!PED::IS_PED_FLEEING(ped) &&
		!AI::IS_PED_CUFFED(ped) &&
		DECORATOR::DECOR_GET_INT(ped, "SH_DUELS_dueled") != 1 &&
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
	if (GAMEPLAY::GET_MISSION_FLAG())
	{
		return;
	}

	if (!duel)
	{
		Entity targetEntity = getPlayerTargetEntity();
		Vector3 playerCoords = playerPos();

		if (targetEntity)
		{
			if (DECISIONEVENT::IS_SHOCKING_EVENT_IN_SPHERE(0x73221D75 /* EVENT_SHOCKING_SEEN_INSULT */, playerCoords.x, playerCoords.y, playerCoords.z, 10))
			{
				DECORATOR::DECOR_SET_INT(targetEntity, "SH_DUELS_duelable", 1);
			}

			challengePrompt->setIsEnabled(!AUDIO::IS_AMBIENT_SPEECH_PLAYING(player));

			if (isPedDuelable(targetEntity))
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
					challengePrompt->hide();
					challengePrompt = new Prompt("Duel", GAMEPLAY::GET_HASH_KEY("INPUT_INTERACT_OPTION1"), PromptMode::Standard);
					challengePrompt->hide();
				}
			}
			else
			{
				challengePrompt->hide();
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

DuelChallengeReaction DuelsEngine::generatePedDuelReaction(Ped candidate)
{
	if (ScriptSettings::getBool("NeverRefuseDuels"))
	{
		return DuelChallengeReaction::Accept;
	}

	float pedAccuracy = PED::GET_PED_ACCURACY(candidate);
	float normalized = (pedAccuracy - 20) / (80 - 20);
	float aggressivenessFactor = abs((float)ScriptSettings::get("AggressivenessTendencyFactor") / 10);
	float acceptanceChance = (normalized + aggressivenessFactor) * 100;
	float random = rndInt(0, 101);
	
	if (acceptanceChance >= random)
	{
		return DuelChallengeReaction::Accept;
	}
	else if (random - (normalized * 100) > 30)
	{
		return DuelChallengeReaction::Flee;
	}
	else 
	{
		return DuelChallengeReaction::Decline;
	}
}