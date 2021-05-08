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
				if (eventType == GAMEPLAY::GET_HASH_KEY("EVENT_PLAYER_COLLECTED_AMBIENT_PICKUP"))
				{
					int eventSize = 36;
					int arr[10];
					SCRIPT::GET_EVENT_DATA(0, i, arr, eventSize);
					showSubtitle(to_string(arr[0]).c_str());

				}

			}

			Vector3 pos = playerPos();

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
					//debug((int)ENTITY::GET_ENTITY_MODEL(e));
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


			//PURSUIT::CLEAR_CURRENT_PURSUIT();
			//PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
			//PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), 0);

			if (IsKeyJustUp(VK_KEY_X))
			{
				//createVehicle(VehicleHash::StageCoach003X, playerPos() + getForwardVector(player) * 10);

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
			}

			if (IsKeyJustUp(VK_KEY_Z))
			{
				Vector3 pos = playerPos() + getForwardVector(player) * rndInt(5, 10);
				Ped ped = createPed("g_m_o_uniexconfeds_01", pos, 180);
				//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
				//playAnimation(ped, "dismantle", "mech_pickup@weapons@dismantle@long_arm", 3000);
				//PED::SET_PED_CONFIG_FLAG(ped, 4, true);
				ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);

				//PED::SET_PED_SPHERE_DEFENSIVE_AREA(ped, pos.x, pos.y, pos.z, 20, 1, 0, 0);
				//AI::TASK_COMBAT_HATED_TARGETS_IN_AREA(ped, pos.x, pos.y, pos.z, 3, 0, 0);


				//Vector3 pos = playerPos();
				//AI::_0x5D5B0D5BC3626E5A(player, -1910137495, GAMEPLAY::GET_HASH_KEY("weapon_revolver_lemat"), ped, 0.22, 0, pos.x, pos.y, pos.z, 180, 1);
			}


			if (IsKeyJustUp(VK_F3))
			{
				Vector3 pos = playerPos() + getForwardVector(player) * 10;
				char* propsetName = "pg_ambient_campfire02xb";
				int propsetHash = GAMEPLAY::GET_HASH_KEY(propsetName);
				PROP::_0xF3DE57A46D5585E9(propsetHash);
				int i = 0;
				while (!PROP::_0x48A88FC684C55FDC(propsetHash) && i < 100)
				{
					i++;
					PROP::_0xF3DE57A46D5585E9(propsetHash);
					WAIT(20);
				}

				if (PROP::_0x48A88FC684C55FDC(propsetHash))
				{
					PROP::_0x899C97A1CCE7D483(propsetHash, pos.x, pos.y, pos.z, 0, 60, 1200, false, true);
				}
				else
				{
					showSubtitle("not loaded");
				}

				PROP::_0xB1964A83B345B4AB(propsetHash);
			}

			if (IsKeyJustUp(VK_KEY_K))
			{
				struct {
					alignas(8) int f_0;
					alignas(8) const char* f_1;
					alignas(8) const char* f_2;
					alignas(8) int f_3;
					alignas(8) int f_4;
					alignas(8) int f_5;
					alignas(8) int f_6;
				} Var13;

				struct {
					alignas(8) int f_0;
					alignas(8) const char* f_1;
					alignas(8) const char* f_2;
					alignas(8) int f_3;
				} Var0;

				Var0.f_0 = -2;
				Var0.f_1 = "Honor_Display_Sounds";
				Var0.f_2 = "Honor_Increase_Big";
				Var0.f_3 = 0;
				Var0.f_0 = 450;
				const char* sParam0 = UI::_CREATE_VAR_STRING(2, "PLAYER_HONOR_CHANGE_POS");
				const char* sParam1 = "ITEMTYPE_TEXTURES";
				int iParam2 = GAMEPLAY::GET_HASH_KEY("TRANSACTION_HONOR_GOOD");

				Var13.f_1 = sParam0;
				Var13.f_2 = sParam1;
				Var13.f_3 = iParam2;
				Var13.f_4 = 1;
				Var13.f_5 = 109029619;
				Var13.f_6 = 0;
				UIUNK::_0xB249EBCB30DD88E0((Any*)&Var0, (Any*)&Var13, 1);
				DUMMY2::_0x74BCCEB233AD95B2(-466562563, GAMEPLAY::GET_HASH_KEY((char*)sParam0));
				DUMMY2::_0x74BCCEB233AD95B2(1885309238, GAMEPLAY::GET_HASH_KEY((char*)sParam1));
				DUMMY2::_0x74BCCEB233AD95B2(-826961056, iParam2);
				//makeHonorEffect(true);


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
		ENTITY::SET_ENTITY_INVINCIBLE(player, true);
	}
	else
	{
		showSubtitle("Debug tools OFF");
		ENTITY::SET_ENTITY_INVINCIBLE(player, false);
	}
}