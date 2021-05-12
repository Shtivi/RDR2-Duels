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

			Hash weaponHash;
			WEAPON::GET_CURRENT_PED_WEAPON(player, &weaponHash, 0, 0, 0);
			if (weaponHash != GAMEPLAY::GET_HASH_KEY("WEAPON_UNARMED")) {
				Entity e;
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &e) /*&& distanceBetweenEntities(player, e) < 20*/) {
					if (IsKeyJustUp(VK_KEY_Z)) {
					}
					debug((int)ENTITY::GET_ENTITY_MODEL(e));
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
					//debug((int)PED::GET_PED_RELATIONSHIP_GROUP_HASH(targetEntity));
					//debug(PED::GET_RELATIONSHIP_BETWEEN_GROUPS(
					//	PED::GET_PED_RELATIONSHIP_GROUP_HASH(player),
					//	PED::GET_PED_RELATIONSHIP_GROUP_HASH(targetEntity)
					//));
					debug(PED::GET_PED_RELATIONSHIP_GROUP_HASH(targetEntity) == GAMEPLAY::GET_HASH_KEY("REL_GANG_DUTCHS"));
					if (IsKeyJustUp(VK_KEY_Z)) {
					}
				}
				else
				{
					debug((int)PED::GET_PED_RELATIONSHIP_GROUP_HASH(player));
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
				//Ped horse = createAmbientHorse(playerPos() + getForwardVector(player) * 5);
				//Ped ped = createPedOnHorse("a_m_m_bivroughtravellers_01", horse, -1);
				//WAIT(2000);
				//AI::_0x48E92D3DDE23C23A(ped, 0, 0, 0, 0, 0);
				//ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&ped);

				Vehicle veh = createVehicle(VehicleHash::Utilliwag, playerPos() + getForwardVector(player) * 10);
				createPed("a_m_m_bivroughtravellers_01", veh, -1);

			}


			if (IsKeyJustUp(VK_F3))
			{
				Ped ped = createPed("a_m_m_bivroughtravellers_01", playerPos() + getForwardVector(player) * 5);
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&ped);
			}

			if (IsKeyJustUp(VK_KEY_K))
			{
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