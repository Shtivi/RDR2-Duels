#include "Main.h"

vector<const char*> ChallengePedsPlayerLines = vector<const char*>
{
	"RE_DUELB_RHD_V1_DB_CALL",
	"RE_DUELB_RHD_V1_INTRO_NEG",
};

vector<const char*> ToTheDuelPlayerLines = vector<const char*>
{
	"RE_DUELB_RHD_V1_TO_THE_DUEL_POS",
	"RE_DUELB_RHD_V1_TO_THE_DUEL_NEG"
};

vector<const char*> PlayerWonDuelLines = vector<const char*>{
	"WITNESS_EXECUTE",
	"PLAYER_TAUNT_SINGLE_ENEMY_NEG_NEAR",
	"RE_DUELB_RHD_V1_PLYFINAL_KILL"
};

bool areMovementControlActive()
{
	return
		CONTROLS::IS_CONTROL_PRESSED(0, 0xD27782E3) || // INPUT_MOVE_DOWN_ONLY
		CONTROLS::IS_CONTROL_PRESSED(0, 0x8FD015D8) || // INPUT_MOVE_UP_ONLY
		CONTROLS::IS_CONTROL_PRESSED(0, 0x7065027D) || // INPUT_MOVE_LEFT_ONLY
		CONTROLS::IS_CONTROL_PRESSED(0, 0xB4E465B4); // INPUT_MOVE_RIGHT_ONLY
}

Duel::Duel(Ped challengedPed, Position pos1, Position pos2)
{
	this->challengedPed = challengedPed;
	this->pos1 = pos1;
	this->pos2 = pos2;
	this->stage = DuelStage::Challenged;
	this->positionBlip = NULL;
	this->opponentDrawTime = 5;
	this->duelCamera = NULL;
	this->duelShockingEvent = 0;
	this->isDuelWellPositioned = false;

	drawPrompt = new Prompt("Draw", GAMEPLAY::GET_HASH_KEY("INPUT_ATTACK"), PromptMode::Mash);
	drawPrompt->setMashModeIncreasePerPress(1.5);
	drawPrompt->setMashModeGrowthSpeed(0.015);
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
			if (playerDistance > 25)
			{
				setStage(DuelStage::PlayerBailed);
			} 
			else if (distance(entityPos(challengedPed), pos2.first) > 20)
			{
				setStage(DuelStage::Declined);
			}

			if (PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(PLAYER::PLAYER_ID(), challengedPed))
			{
				setStage(DuelStage::PlayerDirtyMove);
			}

			if (!positionBlip)
			{
				log("waiting for positioning to be completed");
				onPositioningInitiated();
			}

			if (isPositioningCompleted())
			{
				log("duel psotioning completed");
				deleteBlipSafe(&positionBlip);
				enterDrawMode();
				setStage(DuelStage::Drawing);
			}
			break;
		}
		case DuelStage::Drawing:
		{
			PED::SET_PED_CONFIG_FLAG(challengedPed, 6, 1); // PCF_DontInfluenceWantedLevel
			
			if (ScriptSettings::getBool("OneShotKills"))
			{
				PED::SET_PED_CONFIG_FLAG(challengedPed, 138, 1); // kill in one shot
			}

			if (AI::GET_SEQUENCE_PROGRESS(player) >= 1 && AI::GET_SEQUENCE_PROGRESS(challengedPed) >= 2)
			{
				if (areMovementControlActive())
				{
					log("Duel was abandonded by player");
					setStage(DuelStage::PlayerBailed);
					AI::CLEAR_PED_TASKS(player, 1, 1);
					AI::CLEAR_PED_TASKS(challengedPed, 1, 1);

					if (ScriptSettings::get("AttackOnBailingChance") > rndInt(1, 101))
					{
						onPlayerDirtyMove();
					}

					return;
				}

				if (!drawTimer.isStarted())
				{
					onDrawModeEntered();
				}
				else if (drawTimer.getElapsedSeconds() == opponentDrawTime)
				{
					Hash opponentSidearmWeapon;
					WEAPON::GET_CURRENT_PED_WEAPON(challengedPed, &opponentSidearmWeapon, true, 2, false);
					WEAPON::SET_CURRENT_PED_WEAPON(challengedPed, opponentSidearmWeapon, true, 0, 0, 0);
					AI::TASK_SHOOT_AT_ENTITY(challengedPed, player, -1, -957453492, 0);
					AI::CLEAR_PED_TASKS(player, 1, 1);
					setStage(DuelStage::PostDuel);
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
					onPlayerDrew();
				}
			}
			break;
		}
		case DuelStage::PostDuel:
		{
			if (drawTimer.getElapsedSeconds() == 3)
			{
				setStage(DuelStage::Combat);
				return;
			}

			if (ENTITY::IS_ENTITY_DEAD(challengedPed))
			{
				onDuelWon();
				setStage(DuelStage::PlayerWon);
			}

			if (ScriptSettings::getBool("OneShotDeath") == 1)
			{
				PED::_0xD77AE48611B7B10A(challengedPed, 100);
			}

			break;
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
	DuelStage oldStage = this->stage;
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
		else if (stage == DuelStage::Declined)
		{
			onOpponentDeclined();
		}
		else if (stage == DuelStage::PostDuel)
		{
			drawTimer.start();
		}
	}

	this->stage = stage;
	log(string("duel stage changed from: ").append(to_string((int)oldStage)).append(" to: ").append(to_string((int)this->stage)));
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

	if (ScriptSettings::getBool("EnableConversation"))
	{
		Conversation* conversation = new Conversation();
		// CALLOUT_AT_MALE_ARMED
		conversation->addLine(player, (char*)ChallengePedsPlayerLines.at(rndInt(0, ChallengePedsPlayerLines.size())));
		conversation->play();
	}

	handleDuelReaction(DuelsEngine::generatePedDuelReaction(challengedPed));
}

void Duel::handleDuelReaction(DuelChallengeReaction reaction)
{
	switch (reaction)
	{
		case DuelChallengeReaction::Accept:
		{
			setStage(DuelStage::Accpeted);
			break;
		}
		case DuelChallengeReaction::Decline:
		{
			Conversation conv = Conversation();
			conv.addLine(challengedPed, "IGNORING_YOU");
			conv.play();
			setStage(DuelStage::Declined);
			break;
		}
		case DuelChallengeReaction::Flee:
		{
			Conversation conv = Conversation();
			conv.addLine(challengedPed, "GET_AWAY_FROM_ME");
			conv.play();
			setStage(DuelStage::Declined);
			AI::_0xFD45175A6DFD7CE9(challengedPed, player, 3, 0, 150.0f, 45000, 0);
			break;
		}
	}
}

void Duel::onDuelAccepted()
{
	if (ScriptSettings::getBool("EnableConversation"))
	{
		Conversation* conversation = new Conversation();
		conversation->addLine(challengedPed, "CHALLENGE_THREATEN_ARMED");
		conversation->play();
	}

	generateDuelPosition();	
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(challengedPed, true);
}

void Duel::generateDuelPosition()
{
	Vector3 playerCoords = playerPos();
	Vector3 opponentCoords = entityPos(challengedPed);

	pos1 = getClosestVehicleNode(playerCoords);
	pos2 = getClosestVehicleNode(calculatePointInDirection(pos1.first, pos1.second, 12), true);
	//pos2 = getClosestVehicleNode(playerCoords + getForwardVector(player) * 10, true);
	float nodesDistance = distance(pos1.first, pos2.first);
	isDuelWellPositioned = true;

	if (nodesDistance > 14 || nodesDistance < 6 || distance(pos1.first, playerCoords) > 25)
	{
		isDuelWellPositioned = false;
		if (distance(playerCoords, opponentCoords) > 20)
		{
			setStage(DuelStage::Declined);
			return;
		}
		else
		{
			float playerHeading = ENTITY::GET_ENTITY_HEADING(player);
			pos1 = make_pair(playerCoords, playerHeading);
			Vector3 opponentDuelCoords = opponentCoords + getForwardVector(player) * (10 - distance(playerCoords, opponentCoords));
			pos2 = make_pair(*getSafeCoordForPed(opponentDuelCoords), playerHeading - 180);
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
	AI::TASK_ACHIEVE_HEADING(0, pos2.second, 1000);
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
	duelShockingEvent = DECISIONEVENT::ADD_SHOCKING_EVENT_FOR_ENTITY(2507051957, challengedPed, 0, 30, 35, -1, 20, 1127481344, 0, 0, -1, -1);

	if (ScriptSettings::getBool("EnableConversation") && rndInt(0, 2) == 1)
	{
		playAmbientSpeech(player, (char*)ToTheDuelPlayerLines.at(rndInt(0, ToTheDuelPlayerLines.size())));
	}

	AI::CLEAR_PED_TASKS(player, 0, 0);
	Object seq1;
	AI::OPEN_SEQUENCE_TASK(&seq1);
	AI::TASK_TURN_PED_TO_FACE_ENTITY(0, challengedPed, 1000, 0, 0, 0);
	playAnimation(0, "idle_a", "mini_duel@player@base", -1, 1, -8, 1);
	AI::TASK_LOOK_AT_ENTITY(0, challengedPed, -1, 0, 51, 0);
	AI::CLOSE_SEQUENCE_TASK(seq1);
	AI::TASK_PERFORM_SEQUENCE(player, seq1);

	WEAPON::SET_CURRENT_PED_WEAPON(challengedPed, WeaponHash::Unarmed, true, 0, 0, 0);
	AI::CLEAR_PED_TASKS(challengedPed, 0, 0);
	Object seq2;
	AI::OPEN_SEQUENCE_TASK(&seq2);
	AI::TASK_TURN_PED_TO_FACE_ENTITY(0, player, 2000, 0, 0, 0);
	AI::TASK_LOOK_AT_ENTITY(0, player, -1, 0, 51, 0);
	if (PED::_0x50F124E6EF188B22(challengedPed))
	{
		playAnimation(0, "base_drunk", "mini_duel@challenger@drunk", -1, 1, -8, 1);
	}
	else
	{
		playAnimation(0, "base_npc", "mini_duel@base", -1, 1, -8, 1);
	}
	AI::CLOSE_SEQUENCE_TASK(seq2);
	AI::TASK_PERFORM_SEQUENCE(challengedPed, seq2);

	Hash opponentSidearmWeapon;
	WEAPON::GET_CURRENT_PED_WEAPON(challengedPed, &opponentSidearmWeapon, true, 3, false);
	WEAPON::SET_AMMO_IN_CLIP(challengedPed, opponentSidearmWeapon, WEAPON::GET_MAX_AMMO_IN_CLIP(challengedPed, opponentSidearmWeapon, true));
	WEAPON::SET_CURRENT_PED_WEAPON(challengedPed, opponentSidearmWeapon, false, 0, 0, 0);
}

void Duel::onDrawModeEntered()
{
	log("entering Draw Mode");
	if (ScriptSettings::getBool("EnableDuelCamera"))
	{
		Vector3 offset = getForwardVector(player) * -1.0 + getUpVector(player) * 0.1 + getRightVector(player) * 0.8f;
		duelCamera = new GameCamera(entityPos(player) + getForwardVector(player) * 500, 70);
		duelCamera->attachTo(player, offset, false);
		duelCamera->pointAt(challengedPed);
		GameCamera::setScriptCamsRendering(true);
		UI::DISPLAY_HUD(false);
	}

	if (ScriptSettings::getBool("DisableHonorLoss"))
	{
		DECORATOR::DECOR_SET_INT(challengedPed, "honor_override", 0);
	}

	WAIT(3000);

	if (ScriptSettings::getBool("EnableSoundEffects"))
	{
		AUDIO::PLAY_SOUND_FRONTEND("HUD_DRAW", "HUD_DUEL_SOUNDSET", true, 0);
	}

	opponentDrawTime = rndInt(4, 7);
	drawTimer.start();
}

void Duel::onPlayerDrew()
{
	GameCamera::setScriptCamsRendering(false, true, 500);
	UI::DISPLAY_HUD(true);
	AI::CLEAR_PED_TASKS(player, 1, 1);
	Hash weaponToEquip = NULL;
	WEAPON::GET_CURRENT_PED_WEAPON(player, &weaponToEquip, 0, 2, 0);
	WEAPON::SET_CURRENT_PED_WEAPON(player, weaponToEquip, true, 0, false, false);
	WAIT(500);

	Hash opponentSidearmWeapon;
	WEAPON::GET_CURRENT_PED_WEAPON(challengedPed, &opponentSidearmWeapon, true, 2, false);
	WEAPON::SET_CURRENT_PED_WEAPON(challengedPed, opponentSidearmWeapon, true, 0, 0, 0);
	AI::TASK_SHOOT_AT_ENTITY(challengedPed, player, -1, -957453492, 0);

	//if (ScriptSettings::getBool("AimingAssist")) {
	//	while (!PLAYER::IS_PLAYER_FREE_AIMING(PLAYER::PLAYER_ID()))
	//	{
	//		WAIT(0);
	//		CONTROLS::_SET_CONTROL_NORMAL(0, GAMEPLAY::GET_HASH_KEY("INPUT_AIM"), 1);
	//		PLAYER::SET_PLAYER_FORCED_AIM(PLAYER::PLAYER_ID(), 1, challengedPed, -1, 1);
	//	}
	//}
	//PLAYER::_0xBBA140062B15A8AC(PLAYER::PLAYER_ID()); // dead eye

	setStage(DuelStage::PostDuel);
	DECORATOR::DECOR_SET_INT(challengedPed, "SH_DUELS_dueled", 1);
}

void Duel::onPlayerBailed()
{
	playAmbientSpeech(challengedPed, "WON_DISPUTE");
}

void Duel::onPlayerDirtyMove()
{
	playAmbientSpeech(challengedPed, "GENERIC_INSULT_HIGH_MALE");
	pedEquipBestWeapon(challengedPed);
	WAIT(500);
	AI::TASK_COMBAT_PED(challengedPed, player, 0, 16);
}

void Duel::cleanup()
{
	deleteBlipSafe(&positionBlip);
	drawPrompt->hide();
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(challengedPed, false);
	GameCamera::setScriptCamsRendering(false, true, 500);
	UI::DISPLAY_HUD(true);
}

void Duel::onOpponentDeclined()
{
	playAmbientSpeech(player, "MELEE_ANTAGONIZE_MALE");
}

void Duel::onDuelWon()
{
	if (ScriptSettings::getBool("EnableSoundEffects"))
	{
		playMusic("DUEL_GENERAL_END_OS");
	}

	playAmbientSpeech(player, (char*)PlayerWonDuelLines.at(rndInt(0, PlayerWonDuelLines.size())));
}