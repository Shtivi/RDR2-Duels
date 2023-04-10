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
		PAD::IS_CONTROL_PRESSED(0, 0xD27782E3) || // INPUT_MOVE_DOWN_ONLY
		PAD::IS_CONTROL_PRESSED(0, 0x8FD015D8) || // INPUT_MOVE_UP_ONLY
		PAD::IS_CONTROL_PRESSED(0, 0x7065027D) || // INPUT_MOVE_LEFT_ONLY
		PAD::IS_CONTROL_PRESSED(0, 0xB4E465B4); // INPUT_MOVE_RIGHT_ONLY
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
			if (ENTITY::IS_ENTITY_DEAD(challengedPed))
			{
				onDuelWon();
				setStage(DuelStage::PlayerWon);
			}

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

			if (ScriptSettings::getBool("FleeHorseWhenTooClose") && 
				distance(player, getPlayerPrimaryHorse()) < ScriptSettings::get("HorseFleeDistance") && 
				!PED::IS_PED_FLEEING(getPlayerPrimaryHorse()))
			{
				playerFleeHorse(getPlayerPrimaryHorse());
			}

			if (isPositioningCompleted())
			{
				log("duel psotioning completed");
				deleteBlipSafe(&positionBlip);
				enterDrawMode();
				setStage(DuelStage::Drawing);
			}
			else if (TASK::GET_SEQUENCE_PROGRESS(challengedPed) >= 3)
			{
				if (BUILTIN::TIMERA() > ScriptSettings::get("BailingTimeout") * 0.675 && !DECORATOR::DECOR_GET_INT(challengedPed, "SH_DUELS_waitingReaction"))
				{
					log("duel is about to be abandoned");
					playAmbientSpeech(challengedPed, "MELEE_BRING_IT_ON");
					DECORATOR::DECOR_SET_INT(challengedPed, "SH_DUELS_waitingReaction", 1);
				}
				else if (BUILTIN::TIMERA() > ScriptSettings::get("BailingTimeout"))
				{
					setStage(DuelStage::PlayerBailed);
				}
			}

			break;
		}
		case DuelStage::Drawing:
		{
			if (ENTITY::IS_ENTITY_DEAD(challengedPed))
			{
				onDuelWon();
				setStage(DuelStage::PlayerWon);
			}

			if (TASK::GET_SEQUENCE_PROGRESS(challengedPed) >= 3)
			{
				if (areMovementControlActive())
				{
					log("Duel was abandonded by player");
					setStage(DuelStage::PlayerBailed);
					TASK::CLEAR_PED_TASKS(player, 1, 1);
					TASK::CLEAR_PED_TASKS(challengedPed, 1, 1);

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
				else if (BUILTIN::TIMERA() > opponentDrawTime)
				{
					Hash opponentSidearmWeapon = getPedEquipedWeapon(challengedPed, WeaponAttachPoint::WEAPON_ATTACH_POINT_PISTOL_R);
					WEAPON::SET_CURRENT_PED_WEAPON(challengedPed, opponentSidearmWeapon, true, 0, 0, 0);
					TASK::TASK_SHOOT_AT_ENTITY(challengedPed, player, -1, -957453492, 0);
					TASK::CLEAR_PED_TASKS(player, 1, 1);
					setStage(DuelStage::PostDuel);
				}

				ENTITY::SET_ENTITY_IS_TARGET_PRIORITY(challengedPed, true, 50);

				if (WEAPON::IS_WEAPON_A_GUN(getPedEquipedWeapon(player)))
				{
					onPlayerDrew();
				}
			}
			break;
		}
		case DuelStage::PostDuel:
		{
			LAW::_SET_BOUNTY_HUNTER_PURSUIT_CLEARED();
			PLAYER::SET_PLAYER_FORCED_AIM(PLAYER::PLAYER_ID(), true, challengedPed, -1, 0);
			GameCamera::setScriptCamsRendering(false, true, 500);
			HUD::DISPLAY_HUD(true);
			if (drawTimer.getElapsedSeconds() == 5)
			{
				setStage(DuelStage::Combat);
				return;
			}

			if (ENTITY::IS_ENTITY_DEAD(challengedPed))
			{
				setStage(DuelStage::PlayerWon);
			} 
			else if (getPedEquipedWeapon(challengedPed) == WeaponHash::Unarmed && !PED::IS_PED_FATALLY_INJURED(challengedPed))
			{
				DECORATOR::DECOR_SET_INT(challengedPed, "honor_override", 10);

				Conversation conv = Conversation();
				conv.addLine(challengedPed, "GET_AWAY_FROM_ME");
				conv.play();
				TASK::TASK_FLEE_PED(challengedPed, player, 3, 0, 150.0f, 45000, 0);
				setStage(DuelStage::PlayerWon);
			}

			if (ScriptSettings::getBool("OneShotDeath") == 1)
			{
				PED::SET_PED_TO_PLAYER_WEAPON_DAMAGE_MODIFIER(challengedPed, 100);
			}

			Vector3 loc = playerPos();
			EVENT::REMOVE_ALL_SHOCKING_EVENTS(false);

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
		else if (stage == DuelStage::PlayerWon)
		{
			onDuelWon();
		}
		else if (stage == DuelStage::PostDuel)
		{
			drawTimer.start();
		}
		else if (stage == DuelStage::Combat)
		{
			TASK::TASK_COMBAT_PED(challengedPed, player, 0, 16);
		}
	}

	this->stage = stage;
	log(string("duel stage changed from: ").append(to_string((int)oldStage)).append(" to: ").append(to_string((int)this->stage)));
}

void Duel::onPedChallenged()
{
	TASK::CLEAR_PED_TASKS(challengedPed, 1, 1);
	DECORATOR::DECOR_SET_INT(challengedPed, "SH_DUELS_dueled", 1);

	Object seq;
	TASK::OPEN_SEQUENCE_TASK(&seq);
	TASK::TASK_TURN_PED_TO_FACE_ENTITY(0, player, -1, 0, 0, 0);
	TASK::TASK_LOOK_AT_ENTITY(0, player, -1, -1, 0, 0);
	TASK::CLOSE_SEQUENCE_TASK(seq);
	TASK::TASK_PERFORM_SEQUENCE(challengedPed, seq);

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
			TASK::TASK_FLEE_PED(challengedPed, player, 3, 0, 150.0f, 45000, 0);
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

	if (getPedEquipedWeapon(challengedPed) != WeaponHash::Unarmed)
	{
		WEAPON::SET_CURRENT_PED_WEAPON(challengedPed, WeaponHash::Unarmed, false, 0, 0, 0);
		TASK::TASK_SWAP_WEAPON(challengedPed, 1, 1, 0, 0);
		WAIT(1000);
	}
}

void Duel::generateDuelPosition()
{
	Vector3 playerCoords = playerPos();
	Vector3 opponentCoords = entityPos(challengedPed);

	pos1 = getClosestVehicleNode(playerCoords);
	pos2 = getClosestVehicleNode(calculatePointInDirection(pos1.first, pos1.second, 8), true);
	float nodesDistance = distance(pos1.first, pos2.first);
	isDuelWellPositioned = true;

	if (nodesDistance > 14 || nodesDistance < 6 || distance(pos1.first, playerCoords) > 25)
	{
		isDuelWellPositioned = false;
		//if (distance(playerCoords, opponentCoords) > 20)
		//{
		//	setStage(DuelStage::Declined);
		//	return;
		//}
		//else
		//{
		//	float playerHeading = ENTITY::GET_ENTITY_HEADING(player);
		//	pos1 = make_pair(playerCoords, playerHeading);
		//	Vector3 opponentDuelCoords = opponentCoords + getForwardVector(player) * (8 - distance(playerCoords, opponentCoords));
		//	pos2 = make_pair(*getSafeCoordForPed(opponentDuelCoords), playerHeading - 180);
		//}

		float playerHeading = ENTITY::GET_ENTITY_HEADING(player);
		pos1 = make_pair(playerCoords, playerHeading);
		Vector3 opponentDuelCoords = opponentCoords + getForwardVector(player) * (8 - distance(playerCoords, opponentCoords));
		float dist = distance(opponentDuelCoords, playerPos());
		pos2 = make_pair(*getSafeCoordForPed(opponentDuelCoords), playerHeading - 180);
	}
}

void Duel::onPositioningInitiated()
{
	positionBlip = createBlip(pos1.first, 0xC19DA63);
	showHelpText("You have challenged someone to a duel. Get into position to begin.");

	TASK::CLEAR_PED_TASKS(challengedPed, 1, 1);
	Object seq;
	TASK::OPEN_SEQUENCE_TASK(&seq);

	if (PED::IS_PED_ON_MOUNT(challengedPed))
	{
		TASK::TASK_DISMOUNT_ANIMAL(0, 0, 0, 0, 0, 0);
	}
	else if (PED::IS_PED_IN_ANY_VEHICLE(challengedPed, false))
	{
		TASK::TASK_LEAVE_VEHICLE(0, PED::GET_VEHICLE_PED_IS_USING(challengedPed), 0, 0);
	}

	TASK::TASK_LOOK_AT_ENTITY(0, player, -1, -1, 2048, 3);
	TASK::TASK_FOLLOW_NAV_MESH_TO_COORD(0, pos2.first.x, pos2.first.y, pos2.first.z, 1, -1, 0.8f, 1, 0);
	TASK::TASK_TURN_PED_TO_FACE_COORD(0, pos1.first.x, pos1.first.y, pos1.first.z, 2000);
	if (PED::_IS_PED_DRUNK(challengedPed))
	{
		playAnimation(0, "base_drunk", "mini_duel@challenger@drunk", -1, 1, -8, 1);
	}
	else
	{
		playAnimation(0, "enter_npc", "mini_duel@base", -1, 1, -1, 0);
		playAnimation(0, "idle_a_npc", "mini_duel@base", -1, 1, -1, 0);
	}
	TASK::CLOSE_SEQUENCE_TASK(seq);
	TASK::TASK_PERFORM_SEQUENCE(challengedPed, seq);
	WAIT(200);
	BUILTIN::SETTIMERA(0);
}

bool Duel::isPositioningCompleted()
{
	if (TASK::GET_SEQUENCE_PROGRESS(challengedPed) < 3)
	{
		return false;
	}

	if (distance(playerPos(), pos1.first) > 1.5 || ENTITY::GET_ENTITY_SPEED(player) > 0.2 || !PED::IS_PED_ON_FOOT(player))
	{
		return false;
	}

	return true;
}

void Duel::enterDrawMode()
{
	duelShockingEvent = EVENT::ADD_SHOCKING_EVENT_FOR_ENTITY(0x2CA3408A, challengedPed, 0, 30, 35, -1, 20, 1127481344, 0, 0, -1, -1);

	DuelsEngine::loadAnimDicts();

	Vector3 pos = playerPos();
	TASK::CLEAR_PED_TASKS(player, 0, 0);

	//Object seq;
	//TASK::OPEN_SEQUENCE_TASK(&seq);
	//playAnimation(0, "enter_player", "mini_duel@base", -1, 2, -2);
	//playAnimation(0, "idle_b_player", "mini_duel@base", 1000, 2, -2);
	//TASK::CLOSE_SEQUENCE_TASK(seq);
	//TASK::TASK_PERFORM_SEQUENCE(player, seq);

	TASK::TASK_DUEL(player, /*MISC::GET_HASH_KEY("idle_a")*/2002375312, getPedEquipedWeapon(player, WeaponAttachPoint::WEAPON_ATTACH_POINT_PISTOL_R), challengedPed, 0.22, 0, pos.x, pos.y, pos.z, 180, 1); // Task_duel

	Hash opponentSidearmWeapon = getPedEquipedWeapon(challengedPed, WeaponAttachPoint::WEAPON_ATTACH_POINT_PISTOL_R);
	WEAPON::SET_AMMO_IN_CLIP(challengedPed, opponentSidearmWeapon, WEAPON::GET_MAX_AMMO_IN_CLIP(challengedPed, opponentSidearmWeapon, true));
	WEAPON::SET_CURRENT_PED_WEAPON(challengedPed, opponentSidearmWeapon, false, 0, 0, 0);

	PED::SET_PED_CONFIG_FLAG(challengedPed, 6, 1); // PCF_DontInfluenceWantedLevel
	if (ScriptSettings::getBool("OneShotKills"))
	{
		PED::SET_PED_CONFIG_FLAG(challengedPed, 138, 1); // kill in one shot
	}
}

void Duel::onDrawModeEntered()
{
	log("entering Draw Mode");
	if (ScriptSettings::getBool("EnableDuelCamera"))
	{
		WAIT(500);
		Vector3 offset = getForwardVector(player) * -1.2 + getUpVector(player) * -0.2 + getRightVector(player) * 0.8f;
		duelCamera = new GameCamera(entityPos(player) + offset, 60);
		duelCamera->pointAt(playerPos() + getForwardVector(player) * 8);
		GameCamera::setScriptCamsRendering(true);
		HUD::DISPLAY_HUD(false);
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

	opponentDrawTime = rndInt(3500, 5500);
	BUILTIN::SETTIMERA(0);
	drawTimer.start();
}

void Duel::onPlayerDrew()
{
	Hash opponentSidearmWeapon;
	WEAPON::GET_CURRENT_PED_WEAPON(challengedPed, &opponentSidearmWeapon, true, 2, false);
	WEAPON::SET_CURRENT_PED_WEAPON(challengedPed, opponentSidearmWeapon, true, 0, 0, 0);
	TASK::TASK_SHOOT_AT_ENTITY(challengedPed, player, -1, -957453492, 0);
	setStage(DuelStage::PostDuel);
}

void Duel::onPlayerBailed()
{
	playAmbientSpeech(challengedPed, "WON_DISPUTE");
	TASK::CLEAR_PED_TASKS(challengedPed, 1, 1);
}

void Duel::onPlayerDirtyMove()
{
	playAmbientSpeech(challengedPed, "GENERIC_INSULT_HIGH_MALE");
	pedEquipBestWeapon(challengedPed);
	WAIT(500);
	TASK::TASK_COMBAT_PED(challengedPed, player, 0, 16);
}

void Duel::cleanup()
{
	deleteBlipSafe(&positionBlip);
	PLAYER::SET_PLAYER_FORCED_AIM(PLAYER::PLAYER_ID(), false, 0, 0, 0);;
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(challengedPed, false);
	GameCamera::setScriptCamsRendering(false, true, 500);
	HUD::DISPLAY_HUD(true);
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