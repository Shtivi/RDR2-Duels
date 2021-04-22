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
	initializeLogger();
	log("Duels - 1.0.0 - By Shtivi");
	ScriptSettings::load("Duels.ini", new SettingsMap {
		{"EnableDuelCamera", 1},
		{"AimingAssist", 0},
		{"DisableHonorLoss", 0}
	});
	duels = new DuelsEngine();
}

void main()
{
	WAIT(500);
	Initialize();
	
	while (true)
	{
		debug(ScriptSettings::get("EnableDuelCamera"));

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
				Ped ped = createPed("g_m_m_uniduster_01", playerPos() + getForwardVector(player) * rndInt(5, 10), 180);
				//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);

				

				//Vector3 offset = getForwardVector(player) * -1.0 + getUpVector(player) * 0.1 + getRightVector(player) * 0.8f;
				//GameCamera cam = GameCamera(entityPos(player) + getForwardVector(player) * 500, 70);
				//cam.attachTo(player, offset,  false);
				////cam.setPosition(entityPos(player) + -1 * getForwardVector(player) * 500);
				////cam.setRotation(toVector3(0, 0, 400));
				//cam.pointAt(ped);
				//GameCamera::setScriptCamsRendering(true);
				//cam.setScriptCamsRendering(false);
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
				//UI::DISPLAY_HUD(true);
				Ped ped = createPed("A_M_M_BynRoughTravellers_01", playerPos() + getForwardVector(player) * rndInt(5, 10), 180);
				Vector3 pos = entityPos(ped);
				//DECISIONEVENT::ADD_SHOCKING_EVENT_AT_POSITION(0x2CA3408A, pos.x, pos.y, pos.z, 0, 30, 35, -1, 20, 1127481344, 0);
				DECISIONEVENT::ADD_SHOCKING_EVENT_FOR_ENTITY(2507051957, player, 0, 30, 35, -1, 20, 1127481344, 0, 0, -1, -1);
				WAIT(500);
				ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
				//WAIT(5000);
				//UI::DISPLAY_HUD(false);
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