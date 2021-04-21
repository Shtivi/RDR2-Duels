#include "Main.h"

Stopwatch drawTimer;
int opponentDrawTime = 5;

Duel::Duel(Ped challengedPed, Position pos1, Position pos2)
{
	this->challengedPed = challengedPed;
	this->pos1 = pos1;
	this->pos2 = pos2;
	this->stage = DuelStage::Challenged;
	this->positionBlip = NULL;

	drawPrompt = new Prompt("Draw", GAMEPLAY::GET_HASH_KEY("INPUT_ATTACK"), PromptMode::Mash);
	drawPrompt->setMashModeIncreasePerPress(1.1);
	drawPrompt->setMashModeGrowthSpeed(0.025);
	drawPrompt->setMashModeDecaySpeed(0);
	drawPrompt->setUrgentPulsating(true);
	drawPrompt->hide();
}

void Duel::update()
{
	float playerDistance = distance(playerPos(), pos1.first);

	switch (getStage())
	{
		case DuelStage::Challenged:
		{
			onPedChallenged();
			setStage(DuelStage::Accpeted);
			break;
		}
		case DuelStage::Accpeted:
		{
			onDuelAccepted();
			setStage(DuelStage::Positioning);
			break;
		}
		case DuelStage::Positioning:
		{
			if (playerDistance > 20)
			{
				setStage(DuelStage::PlayerBailed);
			}

			if (PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(PLAYER::PLAYER_ID(), challengedPed))
			{
				setStage(DuelStage::PlayerDirtyMove);
			}

			if (!positionBlip)
			{
				onPositioningInitiated();
			}

			if (isPositioningCompleted())
			{
				deleteBlipSafe(&positionBlip);
				enterDrawMode();
				setStage(DuelStage::Drawing);
			}
			break;
		}
		case DuelStage::Drawing:
		{
			if (AI::GET_SEQUENCE_PROGRESS(player) >= 1 && AI::GET_SEQUENCE_PROGRESS(challengedPed) >= 2)
			{

				if (!drawTimer.isStarted())
				{
					onDrawModeEntered();

					opponentDrawTime = rndInt(4, 7);
					drawTimer.start();
				}

				if (drawTimer.getElapsedSeconds() == opponentDrawTime)
				{
					Hash opponentSidearmWeapon;
					WEAPON::GET_CURRENT_PED_WEAPON(challengedPed, &opponentSidearmWeapon, true, 2, false);
					WEAPON::SET_CURRENT_PED_WEAPON(challengedPed, opponentSidearmWeapon, true, 0, 0, 0);
					AI::TASK_SHOOT_AT_ENTITY(challengedPed, player, -1, -957453492, 0);
				}

				drawPrompt->show();
				if (!drawPrompt->isControlPressed())
				{
					drawPrompt->setMashModeDecaySpeed(2);
				}
				else
				{
					drawPrompt->setMashModeDecaySpeed(0);
				}

				if (drawPrompt->isActivatedByPlayer())
				{
					drawPrompt->hide();
					
					AI::CLEAR_PED_TASKS(player, 1, 1);
					Hash weaponToEquip = NULL;
					WEAPON::GET_CURRENT_PED_WEAPON(player, &weaponToEquip, 0, 2, 0);
					WEAPON::SET_CURRENT_PED_WEAPON(player, weaponToEquip, true, 0, false, false);
					WAIT(500);

					while (!PLAYER::IS_PLAYER_FREE_AIMING(PLAYER::PLAYER_ID()))
					{
						WAIT(0);
						CONTROLS::_SET_CONTROL_NORMAL(0, GAMEPLAY::GET_HASH_KEY("INPUT_AIM"), 1);
						PLAYER::SET_PLAYER_FORCED_AIM(PLAYER::PLAYER_ID(), 1, challengedPed, -1, 1);
					}
					//PLAYER::_0xBBA140062B15A8AC(PLAYER::PLAYER_ID());
					setStage(DuelStage::Combat);
				}
			}
			break;
		}
		case DuelStage::Combat:
		{
		}
	}
}

DuelStage Duel::getStage()
{
	return stage;
}

bool Duel::isRunning()
{
	return (int)getStage() < (int)DuelStage::Combat;
}

void Duel::setStage(DuelStage stage)
{
	if (this->stage != stage)
	{
		if (stage == DuelStage::PlayerBailed)
		{
			onPlayerBailed();
		}
		else if (stage == DuelStage::PlayerDirtyMove)
		{
			onPlayerDirtyMove();
		}
		else if (stage == DuelStage::Drawing)
		{
		}
	}

	this->stage = stage;
}

void Duel::onPedChallenged()
{
	AI::CLEAR_PED_TASKS(challengedPed, 1, 1);

	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_TURN_PED_TO_FACE_ENTITY(0, player, -1, 0, 0, 0);
	AI::TASK_LOOK_AT_ENTITY(0, player, -1, 0, 0, 0);
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(challengedPed, seq);

	Conversation* conversation = new Conversation();
	conversation->addLine(player, "RE_DUELB_RHD_V1_DB_CALL");
	conversation->play();
}

void Duel::onDuelAccepted()
{
	Conversation* conversation = new Conversation();
	conversation->addLine(challengedPed, "CHALLENGE_THREATEN_ARMED");
	conversation->play();
	generateDuelPosition();	
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(challengedPed, true);

}

void Duel::generateDuelPosition()
{
	Vector3 playerCoords = playerPos();
	Vector3 opponentCoords = entityPos(challengedPed);

	pos1 = getClosestVehicleNode(playerCoords);
	pos2 = getClosestVehicleNode(playerCoords + getForwardVector(player) * 12, true);

	if (distance(pos1.first, pos2.first) > 12 || distance(pos1.first, playerCoords) > 20)
	{
		float playerHeading = ENTITY::GET_ENTITY_HEADING(player);
		pos1 = make_pair(playerCoords, playerHeading);
		
		if (distance(player, challengedPed) < 8)
		{
			pos2 = make_pair(opponentCoords, playerHeading - 180);
		}
		else
		{
			pos2 = make_pair(entityPos(challengedPed), playerHeading - 180);
		}
	}
}

void Duel::onPositioningInitiated()
{
	positionBlip = createBlip(pos1.first, 0xC19DA63);
	showHelpText("You have challenged someone to a duel. Get into position to begin.");

	AI::CLEAR_PED_TASKS(challengedPed, 1, 1);
	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_FOLLOW_NAV_MESH_TO_COORD(0, pos2.first.x, pos2.first.y, pos2.first.z, 1, -1, 0.8f, 1, 0);
	AI::TASK_ACHIEVE_HEADING(0, pos2.second -50, 1000);
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(challengedPed, seq);
	WAIT(200);
}

bool Duel::isPositioningCompleted()
{
	if (AI::GET_SEQUENCE_PROGRESS(challengedPed) != -1)
	{
		return false;
	}

	if (distance(playerPos(), pos1.first) > 1.8)
	{
		return false;
	}

	return true;
}

void Duel::enterDrawMode()
{
	AI::CLEAR_PED_TASKS(player, 0, 0);
	Object seq1;
	AI::OPEN_SEQUENCE_TASK(&seq1);
	AI::TASK_TURN_PED_TO_FACE_ENTITY(0, challengedPed, 1000, 0, 0, 0);
	playAnimation(0, "idle_a", "mini_duel@player@base", -1, 1, -8, 1);
	AI::TASK_LOOK_AT_ENTITY(0, challengedPed, -1, 0, 51, 0);
	AI::CLOSE_SEQUENCE_TASK(seq1);
	AI::TASK_PERFORM_SEQUENCE(player, seq1);

	AI::CLEAR_PED_TASKS(challengedPed, 0, 0);
	Object seq2;
	AI::OPEN_SEQUENCE_TASK(&seq2);
	AI::TASK_TURN_PED_TO_FACE_ENTITY(0, player, 2000, 0, 0, 0);
	AI::TASK_LOOK_AT_ENTITY(0, player, -1, 0, 51, 0);
	playAnimation(0, "base_npc", "mini_duel@base", -1, 1, -8, 1);
	AI::CLOSE_SEQUENCE_TASK(seq2);
	AI::TASK_PERFORM_SEQUENCE(challengedPed, seq2);

	Hash opponentSidearmWeapon;
	WEAPON::GET_CURRENT_PED_WEAPON(challengedPed, &opponentSidearmWeapon, true, 3, false);
	WEAPON::SET_AMMO_IN_CLIP(challengedPed, opponentSidearmWeapon, WEAPON::GET_MAX_AMMO_IN_CLIP(challengedPed, opponentSidearmWeapon, true));
	WEAPON::SET_CURRENT_PED_WEAPON(challengedPed, opponentSidearmWeapon, false, 0, 0, 0);
}

void Duel::onDrawModeEntered()
{
	AUDIO::PLAY_SOUND_FRONTEND("HUD_DRAW", "HUD_DUEL_SOUNDSET", true, 0);
	//showHelpText("Hold ~INPUT_AIM~ and ~INPUT_ATTACK~ to draw.");
}

void Duel::onPlayerBailed()
{
	playAmbientSpeech(challengedPed, "WON_DISPUTE");

}

void Duel::onPlayerDirtyMove()
{
	playAmbientSpeech(challengedPed, "GENERIC_INSULT_HIGH_MALE");
	AI::TASK_COMBAT_PED(challengedPed, player, 0, 16);
}

void Duel::cleanup()
{
	deleteBlipSafe(&positionBlip);
	drawPrompt->hide();
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(challengedPed, false);


}