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
	log("Duels by Shtivi - 1.0.0");

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

			//DECISIONEVENT::REMOVE_ALL_SHOCKING_EVENTS(0);
			//DECISIONEVENT::SUPPRESS_SHOCKING_EVENTS_NEXT_FRAME();

			Vector3 pos = playerPos();


			//debug(DECISIONEVENT::IS_SHOCKING_EVENT_IN_SPHERE(0x93B7032F, pos.x, pos.y, pos.z, 60));
			//debug(DECISIONEVENT::IS_SHOCKING_EVENT_IN_SPHERE(1811873798, pos.x, pos.y, pos.z, 60));

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
					//debug(PED::IS_PED_RESPONDING_TO_EVENT(e, -587661767));
					//debug((int)ENTITY::GET_ENTITY_MODEL(e));
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
						/*log(to_string(ENTITY::GET_ENTITY_HEADING(targetEntity)));
						log(to_string((int)ENTITY::GET_ENTITY_MODEL(targetEntity)));*/
						//log((int)PED::GET_PED_RELATIONSHIP_GROUP_HASH(targetEntity));
						//showSubtitle(to_string((int)PED::GET_PED_RELATIONSHIP_GROUP_HASH(targetEntity)).c_str());
					}
					//debug(PED::IS_PED_RESPONDING_TO_EVENT(targetEntity, -587661767));
					//debug(PED::_0xC8D523BF5BBD3808(targetEntity, 0x12AB59DE));

					//debug((int)PED::_0xEC6B59BE445FEC51(targetEntity));
					//debug(PED::GET_PED_COMBAT_MOVEMENT(targetEntity));
					//debug(PED::GET_PED_CONFIG_FLAG(targetEntity, 347, 1));
					//debug(PED::GET_PED_ACCURACY(targetEntity));
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
				Ped ped = createPed("A_M_M_BynRoughTravellers_01", playerPos() + getForwardVector(player) * rndInt(5, 10), 180);
				//PED::SET_PED_CONFIG_FLAG(ped, 138, 1); // kill in one shot
				//PED::SET_PED_CONFIG_FLAG(ped, 6, 1); // PCF_DontInfluenceWantedLevel

				//showSubtitle(to_string(PED::GET_PED_COMBAT_MOVEMENT(ped)).c_str());
				//showSubtitle(to_string(PED::GET_PED_CONFIG_FLAG(ped, 460, 0)).c_str());
				/*WAIT(1000);
				playAmbientSpeech(ped, "IGNORING_YOU");*/

				ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);

				// 0xEC6681EB EVENT_SHOCKING_AUDIBLE_REACTION 


				//while (!ENTITY::IS_ENTITY_DEAD(ped))
				//{
				//	debug("killit");
				//	WAIT(0);
				//}

				//	int bone;
				//	PED::GET_PED_LAST_DAMAGE_BONE(ped, &bone);
				//	if (bone == 21030)
				//	{
				//		showSubtitle("headshot");
				//	}

				////UI::DISPLAY_HUD(true);
				//Ped ped = createPed("A_M_M_BynRoughTravellers_01", playerPos() + getForwardVector(player) * rndInt(5, 10), 180);
				//Vector3 pos = entityPos(ped);
				////DECISIONEVENT::ADD_SHOCKING_EVENT_AT_POSITION(0x2CA3408A, pos.x, pos.y, pos.z, 0, 30, 35, -1, 20, 1127481344, 0);
				//DECISIONEVENT::ADD_SHOCKING_EVENT_FOR_ENTITY(2507051957, player, 0, 30, 35, -1, 20, 1127481344, 0, 0, -1, -1);
				//WAIT(500);
				//ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
				////WAIT(5000);
				////UI::DISPLAY_HUD(false);
			}


			if (IsKeyJustUp(VK_F3))
			{
				AI::CLEAR_PED_TASKS(player, 1, 1);
			}

			if (IsKeyJustUp(VK_KEY_K))
			{
				log("player direction:");
				log(to_string(getForwardVector(player)));
				Position pos1 = getClosestVehicleNode(playerPos());
				//Position pos2 = make_pair(calculatePointInDirection(pos1.first, pos1.second, 10), 0);
				//log(to_string(pos1.second));
				//log(to_string(ENTITY::GET_ENTITY_HEADING(player)));
				Position pos2 = getClosestVehicleNode(calculatePointInDirection(pos1.first, pos1.second, 10), true);

				log(to_string(pos1.first));
				log(to_string(pos2.first));

				Blip b1 = createBlip(pos1.first, BLIP_STYLE_WHITE_DOT);
				Blip b2 = createBlip(pos2.first, BLIP_STYLE_WHITE_DOT);
				WAIT(5000);
				deleteBlipSafe(&b1);
				deleteBlipSafe(&b2);
			}
		}

		if (false && IsKeyJustUp(VK_F2))
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