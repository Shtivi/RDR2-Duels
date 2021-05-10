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
			//debug(PLAYER::GET_PLAYER_CURRENT_STEALTH_NOISE(PLAYER::PLAYER_ID()));

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
						//playEntityAnimation(e, "still_feedfire", "script_story@gry1@ig@ig_1_workerloopworker_b", -1);
						//playEntityAnimation(e, "put_in_still_still", "script_re@moonshine_camp@player_put_in_herbs", 2000);
						GRAPHICS::_0x6EC2A67962296F49(e, 255, 255, 255);
						//GRAPHICS::_0xAB72C67163DC4DB4(e, 20);

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
				Vector3 pos = playerPos() + getForwardVector(player) * 3;
				/*
				Ped ped = createPed("u_m_m_lnsworker_01", pos, 180);
				WAIT(500);
				Entity bucket = createProp("p_basinwater01x", entityPos(ped) + getForwardVector(ped) * 0.5);
				Object seq;
				AI::OPEN_SEQUENCE_TASK(&seq);
				playAnimation(0, "idle_a", "amb_misc@world_human_wash_face_bucket@ground@male_a@idle_a", -1, 1, -8);
				playAnimation(0, "idle_c", "amb_misc@world_human_wash_face_bucket@ground@male_a@idle_a", -1, 8, -8);
				playAnimation(0, "idle_b", "amb_misc@world_human_wash_face_bucket@ground@male_a@idle_a", -1, 8, -1);
				AI::CLOSE_SEQUENCE_TASK(seq);
				AI::TASK_PERFORM_SEQUENCE(ped, seq);

				playEntityAnimation(bucket, "idle_a_bucket", "amb_misc@world_human_wash_face_bucket@ground@male_a@idle_a", -1);
				*/


				// -1118946704 bag
				// 822715387 saddle
				//  -602889539 - body
				// -706113014 body
				// -421224518 - body
				// s_moonshinesack02x
				//OBJECT::CREATE_PORTABLE_PICKUP(GAMEPLAY::GET_HASH_KEY("PICKUP_PORTABLE_PACKAGE"), pos.x, pos.y, pos.z, true, GAMEPLAY::GET_HASH_KEY("p_floursack03x"));

				//Pickup pickup = OBJECT::CREATE_PICKUP(GAMEPLAY::GET_HASH_KEY("PICKUP_PORTABLE_PACKAGE"), pos.x, pos.y, pos.z, 0, 0, 0, GAMEPLAY::GET_HASH_KEY("s_moneysackbig01x"), 0, 0);
				//OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(OBJECT::GET_PICKUP_OBJECT(pickup), true);

				int prop = createProp("s_moonshinesack02x", pos);
				//showSubtitle(to_string(ENTITY::_0x34F008A7E48C496B(prop, 3)).c_str());
				//ENTITY::_0x18FF3110CF47115D(prop, 12, true);
				//ENTITY::_0x18FF3110CF47115D(prop, 3, true);
				//ENTITY::_0x18FF3110CF47115D(prop, 9, true);
				AI::_0xF0B4F759F35CC7F5(prop, -421224518, 0, 6, 0);
				showSubtitle(AI::_0x0CCFE72B43C9CF96(prop) ? "carriable" : "not carriable");
				//AI::_0xF0B4F759F35CC7F5(prop, 1959114253, player, 7, 512);
				//AI::_0xF0B4F759F35CC7F5(prop, -421224518, player, 1, 0);

				//OBJECT::CREATE_PICKUP_ROTATE(GAMEPLAY::GET_HASH_KEY("PICKUP_PORTABLE_PACKAGE"), pos.x, pos.y, pos.z, 0.0, 0.0, 0.0, 8, 1, 24, true, GAMEPLAY::GET_HASH_KEY("p_cs_pelt_large"), 0, 0);




				//Vector3 pos = playerPos();
				//AI::_0x5D5B0D5BC3626E5A(player, -1910137495, GAMEPLAY::GET_HASH_KEY("weapon_revolver_lemat"), ped, 0.22, 0, pos.x, pos.y, pos.z, 180, 1);
			}


			if (IsKeyJustUp(VK_F3))
			{
				// pg_moonshinecampgroup01x
				// pg_re_moonshinecampgroup02x
				// pg_re_davisboysgang01x

				Vector3 pos = playerPos() + getForwardVector(player) * 10;
				char* propsetName = "pg_re_davisboysgang01x";
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
					WAIT(500);

					Ped ped = createPed("u_m_m_lnsworker_01", playerPos() + getForwardVector(player) * rndInt(5, 10), 180);
					Entity still = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 30, GAMEPLAY::GET_HASH_KEY("p_still02x"), 1, 1, 1);
					//createBlip(still, BLIP_STYLE_WHITE_DOT);

					//Vector3 stillCoords = entityPos(still);
					//Object seq;
					//AI::OPEN_SEQUENCE_TASK(&seq);
					//AI::TASK_FOLLOW_NAV_MESH_TO_COORD(0, stillCoords.x, stillCoords.y, stillCoords.z, 1, -1, 0.2, 1, 0);
					//AI::TASK_TURN_PED_TO_FACE_ENTITY(0, still, 1000, 0, 0, 0);
					//playAnimation(0, "put_in_still", "script_re@moonshine_camp@player_put_in_herbs");
					//AI::CLOSE_SEQUENCE_TASK(seq);
					//AI::TASK_PERFORM_SEQUENCE(ped, seq);
					//while (AI::GET_SEQUENCE_PROGRESS(ped) < 2)
					//{
					//	WAIT(0);
					//}
					//playEntityAnimation(still, "put_in_still_still", "script_re@moonshine_camp@player_put_in_herbs", 2000);
				}
				else
				{
					showSubtitle("not loaded");
				}

				PROP::_0xB1964A83B345B4AB(propsetHash);
			}

			if (IsKeyJustUp(VK_KEY_K))
			{
				// Wagon5 + LoadsOfMoonshine
				Vehicle veh = createVehicle(VehicleHash::Wagon05X, playerPos() + getForwardVector(player) * 5, 0);
				//Vehicle veh = createVehicle(GAMEPLAY::GET_HASH_KEY("utilliwag"), playerPos() + getForwardVector(player) * 5, 0);
				setVehicleCargo(veh, VehicleCargoHash::TonsOfMoonshine);
				PROP::_0xC0F0417A90402742(veh, GAMEPLAY::GET_HASH_KEY("pg_teamster_wagon05x_lightupgrade3"));
				
				for (int i = 0; i <= 5; i++)
				{

					VEHICLE::_0xF89D82A0582E46ED (veh, i);
					showSubtitle(to_string(i).c_str());
					WAIT(1000);
				}

				//createProp("p_basinwater01x", playerPos() + getForwardVector(player) * 3);

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