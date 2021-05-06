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
	PLAYER::SET_PLAYER_CONTROL(PLAYER::GET_PLAYER_INDEX(), 1, 0, 0);

	DECORATOR::DECOR_REGISTER("SH_DUELS_dueled", 3);
	DECORATOR::DECOR_REGISTER("SH_DUELS_duelable", 3);
	initializeLogger();
	log("Duels by Shtivi - 1.2.0");

	ScriptSettings::load("Duels.ini", new SettingsMap {
		{"EnableDuelCamera", 1},
		{"DisableHonorLoss", 0},
		{"DuelLawmen", 0},
		{"AttackOnBailingChance", 50},
		{"EnableSoundEffects", 1},
		{"EnableConversation", 1},
		{"OneShotKills", 0},
		{"OneShotDeath", 0},
		{"NeverRefuseDuels", 0},
		{"AggressivenessTendencyFactor", 0},
		{"AntagonizeFirst", 1}
	});

	duels = new DuelsEngine();
}




struct eventData
{
	alignas(8) int promptType;
	alignas(8) int unk1;
	alignas(8) int targetEntityId;
	alignas(8) int unk2;
	alignas(8) int unk3;
	alignas(8) int unk4	;
	alignas(8) int unk5	;
	alignas(8) int discoverableEntityTypeId	;
	alignas(8) int unk6	;
	alignas(8) int emoteHashBin	;
};


void main()
{
	WAIT(500);
	Initialize();
	
	while (true)
	{
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
				showSubtitle(e.what());
				//log(e.what());
			}
		}


		if (debugOn)
		{
			int n = SCRIPT::GET_NUMBER_OF_EVENTS(0);
			for (int i = 0; i < n; i++)
			{
				int eventType = SCRIPT::GET_EVENT_AT_INDEX(0, i);
				if (eventType == GAMEPLAY::GET_HASH_KEY("EVENT_PLAYER_PROMPT_TRIGGERED"))
				{
					int eventSize = 10;
					//int* arr = new int[eventSize];
					int arr[10];
					SCRIPT::GET_EVENT_DATA(0, i, arr, eventSize);
					showSubtitle(to_string(arr[0]).c_str());

				}

			}

			Vector3 pos = playerPos();

			debug(CONTROLS::_IS_INPUT_DISABLED(0));

			//Hash weap;
			//WEAPON::GET_CURRENT_PED_WEAPON(player, &weap, 0, 0, 0);
			//if (weap == GAMEPLAY::GET_HASH_KEY("weapon_revolver_lemat") && !PED::IS_PED_RELOADING(player))
			//{
			//	int ammo;
			//	//WEAPON::GET_AMMO_IN_CLIP(player, GAMEPLAY::GET_HASH_KEY("weapon_revolver_lemat"), &ammo);
			//	int* i = new int;
			//	*i = 0;
			//	WEAPON::_0x678F00858980F516(player, (Any*)&ammo, (Any*)i);
			//	debug(ammo);
			//	//debug(WEAPON::GET_AMMO_IN_PED_WEAPON(player, GAMEPLAY::GET_HASH_KEY("weapon_revolver_lemat")));
			//}

			Hash weaponHash;
			WEAPON::GET_CURRENT_PED_WEAPON(player, &weaponHash, 0, 0, 0);
			if (weaponHash != GAMEPLAY::GET_HASH_KEY("WEAPON_UNARMED")) {
				Entity e;
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &e) /*&& distanceBetweenEntities(player, e) < 20*/) {
					if (IsKeyJustUp(VK_KEY_Z)) {
						int bone;
						PED::GET_PED_LAST_DAMAGE_BONE(e, &bone);
						PED::_0xFFD54D9FE71B966A(e, 2, 26043, -.5, -.05, 0, ENTITY::GET_ENTITY_HEADING(e), 5000, -1, 1);
					}
				}
				else
				{
				}
			}
			else
			{
				Entity targetEntity;
				if (PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &targetEntity))
				{
					if (IsKeyJustUp(VK_KEY_Z)) {
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
			//PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), 0);

			if (IsKeyJustUp(VK_KEY_X))
			{
				playAmbientSpeech(player, "RE_ES_EMR_V1_ACCEPT_P");
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
				Ped ped = createPed("g_m_o_uniexconfeds_01", playerPos() + getForwardVector(player) * rndInt(5, 10), 180);
				//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
				//ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);

				//WAIT(500);
				//while (STREAMING::HAS_ANIM_DICT_LOADED("MINI_DUEL@PLAYER@BASE"))
				//{
				//	STREAMING::REQUEST_ANIM_DICT("MINI_DUEL@PLAYER@BASE");
				//	WAIT(10);
				//}

				//Vector3 pos = playerPos();
				//AI::_0x5D5B0D5BC3626E5A(player, -1910137495, GAMEPLAY::GET_HASH_KEY("weapon_revolver_lemat"), ped, 0.22, 0, pos.x, pos.y, pos.z, 180, 1);
			}


			if (IsKeyJustUp(VK_F3))
			{
				AI::CLEAR_PED_TASKS(player, 1, 1);
			}

			if (IsKeyJustUp(VK_KEY_K))
			{
				PURSUIT::_0xDE5FAA741A781F73(PLAYER::GET_PLAYER_INDEX(), 0);
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