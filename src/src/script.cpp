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

Ped ped = 0;

void Initialize()
{
	player = PLAYER::PLAYER_PED_ID();
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

			if (ped) {
				int prog = AI::GET_SEQUENCE_PROGRESS(ped);
				debug(prog);
				//if (prog == 1) 
				//{
				//	giveWeaponToPed(ped, WeaponHash::RevolverCattleman, 90, true);
				//}
			}

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

					//debug(PED::_0xB655DB7582AEC805(e));
					debug((int)ENTITY::GET_ENTITY_MODEL(e));
				}
				else
				{
					//RaycastResult ray = raycastCrosshair(500);
					//if (ray.hitEntity) 
					//{
					//	debug((int)ENTITY::GET_ENTITY_MODEL((int)ray.hitEntity));
					//}
					//else
					//{
					//	debug(to_string(ray.hitPos));
					//}
					//if (IsKeyJustUp(VK_KEY_Z))
					//{
					//	log(ray.hitPos);
					//	log(to_string(ENTITY::GET_ENTITY_HEADING(player)));
					//}
					//if (IsKeyJustUp(VK_F3))
					//{
					//	//new CampingSite(ray.hitPos, 25, 8, CampVariant::Campfire);
					//}
				}
			}
			else
			{
				Entity targetEntity;
				if (PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &targetEntity))
				{
					if (IsKeyJustUp(VK_KEY_Z)) {
						log(to_string(ENTITY::GET_ENTITY_HEADING(targetEntity)));
						log(to_string((int)ENTITY::GET_ENTITY_MODEL(targetEntity)));
					}
					//debug((int)ENTITY::GET_ENTITY_MODEL(targetEntity));
					//debug(PED::GET_PED_TYPE(targetEntity));
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
				ped = createPed("g_m_m_uniduster_01", playerPos() + getForwardVector(player) * 5, 180);
				//Ped ped2 = createPed("g_m_m_uniduster_01", playerPos() + getForwardVector(player) * 10);
				PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
				//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped2, true);

				WAIT(1000);

				//pedEquipBestWeapon(ped);
				//pedEquipBestWeapon(ped2);

				//AI::_0x5D5B0D5BC3626E5A(ped, WEAPON::GET_BEST_PED_WEAPON(ped, 0, 0), WEAPON::GET_BEST_PED_WEAPON(ped2, 0, 0), ped2, 0.22f, 0, 0, 0, 0, 90, 1);

				//playAnimation(ped, "idle_a", "mini_duel@challenger@rc@rcal@rc3_ig1", 4000);
				Vector3 shootAt = entityPos(ped) + getForwardVector(ped) * 50;
				WEAPON::REMOVE_ALL_PED_WEAPONS(ped, 1, 1);
				WAIT(500);
				giveWeaponToPed(ped, WeaponHash::RevolverCattleman, 90, true);
				Object seq;
				AI::OPEN_SEQUENCE_TASK(&seq);
				playAnimation(0, "enter_npc", "mini_duel@base", 4000);
				//playAnimation(0, "idle_c_npc", "mini_duel@base", 2000);
				//playAnimation(0, "hard_c_draw_npc", "mini_duel@base", 2000);
				//playAnimation(0, "hard_c_fire_npc", "mini_duel@base", -1);
				AI::TASK_SHOOT_AT_COORD(0, shootAt.x, shootAt.y, shootAt.z, 5000, 1566631136, 0);
				//playAnimation(0, "enter", "mini_duel@challenger@rc@rcal@rc3_ig1", 2000);
				//playAnimation(0, "idle_a", "mini_duel@challenger@rc@rcal@rc3_ig1", 2000);
				//playAnimation(0, "fast_draw_a", "mini_duel@challenger@rc@rcal@rc3_ig1", 2000);
				AI::CLOSE_SEQUENCE_TASK(seq);
				AI::TASK_PERFORM_SEQUENCE(ped, seq);
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
			}

			if (IsKeyJustUp(VK_F3))
			{
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
	}
	else
	{
		showSubtitle("Debug tools OFF");
	}
}