/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "Main.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Ped player;

bool debugOn = false;
bool invincible = false;

DuelsEngine* duels;

void Initialize()
{
	player = PLAYER::PLAYER_PED_ID();
	duels = new DuelsEngine();
}

void main()
{
	WAIT(500);
	Initialize();
	
	
	//UI::_0xCD1BDFF15EFA79F5(mashP->handle, 1, 0.8, 0); //_UIPROMPT_SET_MASH_WITH_RESISTANCE_MODE
	//UI::_0xF957A1654C6322FE(mashP->handle, 0); //_UIPROMPT_SET_BEAT_MODE
	//UI::_0x5F6503D9CD2754EB(mashP->handle, 0.5, 1, 0); //_UIPROMPT_SET_TARGET_MODE
	//UI::_0x1473D3AF51D54276(mashP->handle, 0); // _UIPROMPT_SET_PRESSED_TIMED_MODE
	//UI::_0x1473D3AF51D54276(mashP->handle, 0); // _UIPROMPT_SET_PRESSED_TIMED_MODE
	//UI::_0x74C7D7B72ED0D3CF(mashP->handle, 10); //_UIPROMPT_SET_STANDARDIZED_HOLD_MODE
	//UI::_0x7B66E89312727274(mashP->handle); //_UIPROMPT_SET_HOLD_INDEFINITELY_MODE

	//UI::_0xDF6423BF071C7F71(mashP->handle, 10); // _UIPROMPT_SET_MASH_MODE
	//UI::_0xF4A5C4509BF923B1(mashP->handle, 1);

	//UI::_0x32DF729D8BD3C1C6(mashP->handle, 1, 0, 0, 0); // _UIPROMPT_SET_MASH_MANUAL_MODE
	//UI::_0xA0D1D79C6036A855(mashP->handle, 1); //_UIPROMPT_SET_MASH_MANUAL_MODE_INCREASE_PER_PRESS
	//UI::_0x56DBB26F98582C29(mashP->handle, 0.015); //_UIPROMPT_SET_MASH_MANUAL_MODE_PRESSED_GROWTH_SPEED
	//UI::_0x7D393C247FB9B431(mashP->handle, 0); // _UIPROMPT_SET_MASH_MANUAL_MODE_DECAY_SPEED
	
	while (true)
	{
		//debug((float)UI::_0x8A9585293863B8A5(mashP->handle));

		player = PLAYER::PLAYER_PED_ID();
		try
		{
			updateUI();
			duels->update();
		}
		catch (...)
		{
			log("Something wrong happened");
			std::exception_ptr ex = std::current_exception();
			try 
			{
				if (ex)
				{
					rethrow_exception(ex);
				}
				else
				{
					log("No exception captured.");
				}
			}
			catch (const exception& e)
			{
				log(e.what());
			}
		}


		if (debugOn)
		{

			Hash weaponHash;
			WEAPON::GET_CURRENT_PED_WEAPON(player, &weaponHash, 0, 0, 0);
			if (weaponHash != GAMEPLAY::GET_HASH_KEY("WEAPON_UNARMED")) {
				Entity e;
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &e) /*&& distanceBetweenEntities(player, e) < 20*/) {
					if (IsKeyJustUp(VK_KEY_Z)) {
						log(to_string((int)ENTITY::GET_ENTITY_MODEL(e)));
						log(to_string(entityPos(e)));
						showSubtitle(to_string(entityPos(e)));
					}

					debug((int)ENTITY::GET_ENTITY_MODEL(e));
				}
				else
				{
				}
			}
			else
			{
				/*CONTROLS::_SET_CONTROL_NORMAL(0, GAMEPLAY::GET_HASH_KEY("INPUT_INTERACT_LOCKON"), 1);
				CONTROLS::_SET_CONTROL_NORMAL(0, GAMEPLAY::GET_HASH_KEY("INPUT_ATTACK"), 1);*/

				Entity targetEntity;
				if (PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &targetEntity))
				{
					if (IsKeyJustUp(VK_KEY_Z)) {
						log(to_string(ENTITY::GET_ENTITY_HEADING(targetEntity)));
						log(to_string((int)ENTITY::GET_ENTITY_MODEL(targetEntity)));
					}
				}
				else
				{
				}
			}

			if (PED::IS_PED_ON_MOUNT(player))
			{

				ENTITY::SET_ENTITY_INVINCIBLE(PED::GET_MOUNT(player), 1);
			}


			PURSUIT::CLEAR_CURRENT_PURSUIT();
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), 0);

			if (IsKeyJustUp(VK_KEY_X))
			{
				Ped ped = createPed("g_m_m_uniduster_01", playerPos() + getForwardVector(player) * 5, 180);



				//PLAYER::_0xBBA140062B15A8AC(PLAYER::PLAYER_ID());
				//Ped ped2 = createPed("g_m_m_uniduster_01", playerPos() + getForwardVector(player) * 10);
				//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
				//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped2, true);


				////pedEquipBestWeapon(ped);
				////pedEquipBestWeapon(ped2);

				////AI::_0x5D5B0D5BC3626E5A(ped, WEAPON::GET_BEST_PED_WEAPON(ped, 0, 0), WEAPON::GET_BEST_PED_WEAPON(ped2, 0, 0), ped2, 0.22f, 0, 0, 0, 0, 90, 1);

				////playAnimation(ped, "idle_a", "mini_duel@challenger@rc@rcal@rc3_ig1", 4000);
				//Vector3 shootAt = entityPos(ped) + getForwardVector(ped) * 50;
				//WEAPON::REMOVE_ALL_PED_WEAPONS(ped, 1, 1);
				//WAIT(500);
				//giveWeaponToPed(ped, WeaponHash::RevolverCattleman, 90, true);
				
				//Object seq;
				//AI::OPEN_SEQUENCE_TASK(&seq);
				////playAnimation(0, "enter_npc", "mini_duel@base", 0);
				//WAIT(2000);
				////playAmbientSpeech(ped, "GENERIC_INSULT_HIGH_MALE");
				//playAnimation(0, "idle_a", "mini_duel@player@base", -1, 2, 8, 0);
				//playAnimation(0, "slow_draw_sweep_up", "mini_duel@player@base", -1, 2, 8, 1);
				////playAnimation(0, "hard_c_draw_npc", "mini_duel@base", 2000);
				////playAnimation(0, "hard_c_fire_npc", "mini_duel@base", -1);
				////AI::TASK_SHOOT_AT_COORD(0, shootAt.x, shootAt.y, shootAt.z, 5000, 1566631136, 0);
				//////playAnimation(0, "enter", "mini_duel@challenger@rc@rcal@rc3_ig1", 2000);
				//////playAnimation(0, "idle_a", "mini_duel@challenger@rc@rcal@rc3_ig1", 2000);
				//////playAnimation(0, "fast_draw_a", "mini_duel@challenger@rc@rcal@rc3_ig1", 2000);
				//AI::CLOSE_SEQUENCE_TASK(seq);
				//AI::TASK_PERFORM_SEQUENCE(ped, seq);
			}

			if (IsKeyJustUp(VK_F1))
			{
				invincible = !invincible;
				if (invincible) {
					showSubtitle("invincible ON");
				}
				else
				{
					showSubtitle("invincible OFF");

				}
				ENTITY::SET_ENTITY_INVINCIBLE(player, invincible);
			}

			if (IsKeyJustUp(VK_KEY_Z))
			{
				//AUDIO::PLAY_SOUND_FRONTEND("HUD_DRAW", "HUD_DUEL_SOUNDSET", true, 0);

				Ped ped = createPed("g_m_m_uniduster_01", playerPos() + getForwardVector(player) * 5, 180);
				PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
				Ped ped2 = createPed("A_M_M_BynRoughTravellers_01", playerPos() + getForwardVector(player) * 10, 180);

				WAIT(1000);

				Hash opponentSidearmWeapon;
				WEAPON::GET_CURRENT_PED_WEAPON(ped, &opponentSidearmWeapon, true, 2, false);
				WEAPON::SET_AMMO_IN_CLIP(ped, opponentSidearmWeapon, WEAPON::GET_MAX_AMMO_IN_CLIP(ped, opponentSidearmWeapon, true));
				WEAPON::SET_CURRENT_PED_WEAPON(ped, opponentSidearmWeapon, true, 0, 0, 0);


				AI::TASK_SHOOT_AT_ENTITY(ped, ped2, -1, -957453492, 0);

			}

			if (IsKeyJustUp(VK_F3))
			{
				AI::CLEAR_PED_TASKS(player, 1, 1);
			}

			if (IsKeyJustUp(VK_KEY_K))
			{

				//PLAYER::_0xBBA140062B15A8AC(PLAYER::PLAYER_ID());

			}
		}

		if (true && IsKeyJustUp(VK_F2))
		{
			setDebugMode(!isDebugMode());
		}

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}

void debug(const char* text) {
	UI::DRAW_TEXT((char*)UI::_CREATE_VAR_STRING(10, "LITERAL_STRING", text), 0, 0);
}

void debug(string text) {
	debug(text.c_str());
}

void debug(Vector3 pos) {
	stringstream str;
	str << pos.x << ", " << pos.y << ", " << pos.z;
	debug(str.str());
}

void debug(float f) {
	debug(to_string(f));
}

void debug(int n) {
	debug(to_string(n));
}

void debug(bool b) {
	debug(to_string(b));
}

void logPlayerPos()
{
	Vector3 playerPos = entityPos(player);
	float ground;
	GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(playerPos.x, playerPos.y, playerPos.z, &ground, false);
	std::stringstream output;
	output << "\n"
		<< playerPos.x << ", " << playerPos.y << ", " << playerPos.z << "\n"
		<< playerPos.x << ", " << playerPos.y << ", " << ground << "\n"
		<< "heading: " << ENTITY::GET_ENTITY_HEADING(player);

	log(output.str().c_str());
	showSubtitle(to_string(getGroundPos(playerPos)));
}

bool isDebugMode()
{
	return debugOn;
}

void setDebugMode(bool toggle)
{
	debugOn = toggle;
	if (debugOn)
	{
		showSubtitle("Debug tools ON");
	}
	else
	{
		showSubtitle("Debug tools OFF");
	}
}