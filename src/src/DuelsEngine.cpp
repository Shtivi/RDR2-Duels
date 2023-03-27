#include "Main.h"

const float DuelDistance = 12;

Prompt* challengePrompt;
Duel* duel;
vector<const char*> animDicts = {
	"MINI_DUEL@PLAYER@BASE",
	"MINI_DUEL@BASE",
	"MINI_DUEL@REPOSITION@BASE",
	"MINI_DUEL@PLAYER@ACTION",
	"MINI_DUEL@PLAYER@NORMAL",
	"MINI_DUEL@PLAYER@MISSION@MUD4",
	"MINI_DUEL@CHALLENGER@BASE",
	"MINI_DUEL@PLAYER@MISSION@IND3@IG9",
	"MINI_DUEL@CHALLENGER@RC@RCAL@RC3_IG1",

};

bool isPedDuelable(Ped ped)
{
	if (ScriptSettings::getBool("AntagonizeFirst") && DECORATOR::DECOR_GET_INT(ped, "SH_DUELS_duelable") != 1)
	{
		return false;
	}

	if (!ScriptSettings::getBool("ChallengeGangMembers") && PED::GET_PED_RELATIONSHIP_GROUP_HASH(ped) == GAMEPLAY::GET_HASH_KEY("REL_GANG_DUTCHS"))
	{
		return false;
	}

	bool allow =
		!ENTITY::IS_ENTITY_DEAD(ped) &&
		PED::IS_PED_HUMAN(ped) &&
		!PED::IS_PED_IN_ANY_TRAIN(ped) &&
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
	challengePrompt = 0;
	DuelsEngine::loadAnimDicts();
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
		int step = 0;

		if (targetEntity)
		{
			if (!challengePrompt)
			{
				challengePrompt = new Prompt("Duel", GAMEPLAY::GET_HASH_KEY("INPUT_INTERACT_OPTION1"), PromptMode::Standard);
			}
			
			challengePrompt->hide();
			challengePrompt->setTargetEntity(targetEntity);

			if (DECISIONEVENT::IS_SHOCKING_EVENT_IN_SPHERE(0x73221D75 /* EVENT_SHOCKING_SEEN_INSULT */, playerCoords.x, playerCoords.y, playerCoords.z, 10))
			{
				DECORATOR::DECOR_SET_INT(targetEntity, "SH_DUELS_duelable", 1);
				log("antagonized");
			}

			challengePrompt->setIsEnabled(!AUDIO::IS_ANY_SPEECH_PLAYING(player));

			if (isPedDuelable(targetEntity))
			{
				challengePrompt->show();

				if (challengePrompt->isActivatedByPlayer())
				{
					log("Duel initiated");
					duel = new Duel(
						targetEntity,
						getClosestVehicleNode(playerPos()),
						getClosestVehicleNode(playerPos() + getForwardVector(player) * DuelDistance, true)
					);

					UI::_0x00EDE88D4D13CF59(challengePrompt->handle); // remove old prompt
					delete challengePrompt;
					challengePrompt = 0;
				}
			}
			else if (challengePrompt)
			{
				UI::_0x00EDE88D4D13CF59(challengePrompt->handle); // remove old prompt
				delete challengePrompt;
				challengePrompt = 0;
			}
		}
		else if (challengePrompt)
		{
			UI::_0x00EDE88D4D13CF59(challengePrompt->handle); // remove old prompt
			delete challengePrompt;
			challengePrompt = 0;
		}
	}
	else
	{
		if (challengePrompt)
		{
			UI::_0x00EDE88D4D13CF59(challengePrompt->handle); // remove old prompt
			delete challengePrompt;
			challengePrompt = 0;
		}

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

void DuelsEngine::loadAnimDicts()
{
	while (!hasAnimDictsLoaded())
	{
		for (const char* animDict : animDicts)
		{
			STREAMING::REQUEST_ANIM_DICT((char*)animDict);
		}
		WAIT(20);
	}

	log("anim dicts loaded");
}

bool DuelsEngine::hasAnimDictsLoaded()
{
	for (const char* animDict : animDicts)
	{
		if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)animDict)) 
		{
			return false;
		}
	}

	return true;
}